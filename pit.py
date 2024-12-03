
from math import sqrt

import torch
from torch import nn, einsum
import torch.nn.functional as F

from einops import rearrange, repeat
from einops.layers.torch import Rearrange


# helpers

def cast_tuple(val, num):
    return val if isinstance(val, tuple) else (val,) * num

def conv_output_size(image_size, kernel_size, stride, padding = 0):
    return int(((image_size - kernel_size + (2 * padding)) / stride) + 1)

# classes

class FeedForward(nn.Module):
    def __init__(self, dim, hidden_dim, dropout = 0.):
        super().__init__()
        self.net = nn.Sequential(
            nn.LayerNorm(dim),
            nn.Linear(dim, hidden_dim),
            nn.GELU(),
            nn.Dropout(dropout),
            nn.Linear(hidden_dim, dim),
            nn.Dropout(dropout)
        )
    def forward(self, x):
        return self.net(x)

class Attention(nn.Module):
    def __init__(self, dim, heads = 8, dim_head = 64, dropout = 0.):
        super().__init__()
        inner_dim = dim_head *  heads
        project_out = not (heads == 1 and dim_head == dim)

        self.heads = heads
        self.scale = dim_head ** -0.5

        self.norm = nn.LayerNorm(dim)
        self.attend = nn.Softmax(dim = -1)
        self.dropout = nn.Dropout(dropout)
        self.to_qkv = nn.Linear(dim, inner_dim * 3, bias = False)

        self.to_out = nn.Sequential(
            nn.Linear(inner_dim, dim),
            nn.Dropout(dropout)
        ) if project_out else nn.Identity()

    def forward(self, x):
        b, n, _, h = *x.shape, self.heads

        x = self.norm(x)
        qkv = self.to_qkv(x).chunk(3, dim = -1)
        q, k, v = map(lambda t: rearrange(t, 'b n (h d) -> b h n d', h = h), qkv)

        dots = einsum('b h i d, b h j d -> b h i j', q, k) * self.scale

        attn = self.attend(dots)
        attn = self.dropout(attn)

        out = einsum('b h i j, b h j d -> b h i d', attn, v)
        out = rearrange(out, 'b h n d -> b n (h d)')
        return self.to_out(out)

class Transformer(nn.Module):
    def __init__(self, dim, depth, heads, dim_head, mlp_dim, dropout = 0.):
        super().__init__()
        self.layers = nn.ModuleList([])
        for _ in range(depth):
            self.layers.append(nn.ModuleList([
                Attention(dim, heads = heads, dim_head = dim_head, dropout = dropout),
                FeedForward(dim, mlp_dim, dropout = dropout)
            ]))
    def forward(self, x):
        for attn, ff in self.layers:
            x = attn(x) + x
            x = ff(x) + x
        return x

# depthwise convolution, for pooling

class DepthWiseConv2d(nn.Module):
    def __init__(self, dim_in, dim_out, kernel_size, padding, stride, bias = True):
        super().__init__()
        self.net = nn.Sequential(
            nn.Conv2d(dim_in, dim_out, kernel_size = kernel_size, padding = padding, groups = dim_in, stride = stride, bias = bias),
            nn.Conv2d(dim_out, dim_out, kernel_size = 1, bias = bias)
        )
    def forward(self, x):
        return self.net(x)

# pooling layer

class Pool(nn.Module):
    def __init__(self, dim):
        super().__init__()
        self.downsample = DepthWiseConv2d(dim, dim * 2, kernel_size = 3, stride = 2, padding = 1)
        self.cls_ff = nn.Linear(dim, dim * 2)

    def forward(self, x):
        cls_token, tokens = x[:, :1], x[:, 1:]

        cls_token = self.cls_ff(cls_token)

        tokens = rearrange(tokens, 'b (h w) c -> b c h w', h = int(sqrt(tokens.shape[1])))
        tokens = self.downsample(tokens)
        tokens = rearrange(tokens, 'b c h w -> b (h w) c')

        return torch.cat((cls_token, tokens), dim = 1)

# main class

class PiT(nn.Module):
    def __init__(
            self,
            *,
            image_size=9,  # 图像大小修改为 9
            patch_size=3,  # 补丁大小修改为 3
            num_classes=1125,  # 第一个任务的输出为 1x1125
            dim=64,  # 维度可根据需要调整
            depth=(4,),  # Transformer 层数（可以根据需要修改）
            heads=(8,),  # 注意力头的数量
            mlp_dim=256,  # MLP 的维度（可以调整）
            dim_head=64,
            dropout=0.,
            emb_dropout=0.,
            channels=32  # 输入通道数修改为 32
    ):
        super().__init__()
        assert image_size % patch_size == 0, '图像尺寸必须能被补丁大小整除。'
        assert isinstance(depth, tuple), 'depth 必须是一个元组，指定每个降采样阶段的块数'
        heads = cast_tuple(heads, len(depth))

        patch_dim = channels * patch_size ** 2  # 补丁的维度改为 channels * patch_size ** 2

        # 修改为处理 batch x 32 x 9 x 9 输入
        self.to_patch_embedding = nn.Sequential(
            nn.Conv2d(channels, dim, kernel_size=patch_size, stride=patch_size // 2),  # 使用卷积进行补丁嵌入
            Rearrange('b c h w -> b (h w) c')  # 将卷积后的输出重新排列为 (batch, num_patches, dim)
        )

        output_size = conv_output_size(image_size, patch_size, patch_size // 2)
        num_patches = output_size ** 2

        self.pos_embedding = nn.Parameter(torch.randn(1, num_patches + 1, dim))
        self.cls_token = nn.Parameter(torch.randn(1, 1, dim))
        self.dropout = nn.Dropout(emb_dropout)

        layers = []

        for ind, (layer_depth, layer_heads) in enumerate(zip(depth, heads)):
            not_last = ind < (len(depth) - 1)

            layers.append(Transformer(dim, layer_depth, layer_heads, dim_head, mlp_dim, dropout))

            if not_last:
                layers.append(Pool(dim))
                dim *= 2

        self.layers = nn.Sequential(*layers)

        # 输出层
        self.mlp_head_1125 = nn.Sequential(
            nn.LayerNorm(dim),
            nn.Linear(dim, 1125)  # 第一个输出，大小为 1125
        )

        self.mlp_head_tanh = nn.Sequential(
            nn.LayerNorm(dim),
            nn.Linear(dim, 1),
            nn.Tanh()  # 第二个输出，应用 Tanh 激活函数
        )

    def forward(self, img):
        # 输入 img 形状为 (batch, 32, 9, 9)
        x = self.to_patch_embedding(img)  # 输出形状为 (batch, num_patches, dim)
        b, n, _ = x.shape

        # 添加类标记 (cls_token)
        cls_tokens = repeat(self.cls_token, '() n d -> b n d', b=b)
        x = torch.cat((cls_tokens, x), dim=1)  # 将 cls_token 加入到输入中
        x += self.pos_embedding[:, :n + 1]  # 添加位置编码
        x = self.dropout(x)

        x = self.layers(x)

        # 获取两个输出
        output_1125 = self.mlp_head_1125(x[:, 0])  # 第一个输出（1x1125）
        output_tanh = self.mlp_head_tanh(x[:, 0])  # 第二个输出（1x1，并应用 Tanh 激活）

        return output_1125, output_tanh


if __name__ == '__main__':
    net = PiT(image_size=9,patch_size=3,num_classes=1125,dim=128,depth=(3,3),heads=4, mlp_dim=256)

    policy,value = net(torch.zeros((10,32,9,9)))


    print(net)

    print(policy.shape)
    print(value.shape)

