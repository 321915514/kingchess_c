# -*- coding: utf-8 -*-
"""
An implementation of the policyValueNet in PyTorch
Tested in PyTorch 0.2.0 and 0.3.0

@author: Junxiao Song
"""

import torch
import torch.nn as nn
import torch.optim as optim
import torch.nn.functional as F
from torch.cuda.amp import autocast
from torch.autograd import Variable
import numpy as np
import torchvision.models as models
from torch.nn.utils.rnn import pad_sequence

# from net.encoder import encoder_board
# from fundamental.board import GameState

def get_latest_opset():
    """Return second-most (for maturity) recently supported ONNX opset by this version of torch."""
    a = max(int(k[14:]) for k in vars(torch.onnx) if "symbolic_opset" in k) - 1
    print(a)
    return max(int(k[14:]) for k in vars(torch.onnx) if "symbolic_opset" in k) - 1  # opset



def set_learning_rate(optimizer, lr):
    """Sets the learning rate to the given value"""
    for param_group in optimizer.param_groups:
        param_group['lr'] = lr


# class Net(nn.Module):
#     """policy-value network module"""
#
#     def __init__(self, board_width, board_height):
#         super(Net, self).__init__()
#
#         self.board_width = board_width
#         self.board_height = board_height
#         # common layers
#         self.conv1 = nn.Conv2d(3, 32, kernel_size=3, padding=1)
#         self.conv2 = nn.Conv2d(32, 64, kernel_size=3, padding=1)
#         self.conv3 = nn.Conv2d(64, 128, kernel_size=3, padding=1)
#         # action policy layers
#         self.act_conv1 = nn.Conv2d(128, 3, kernel_size=1)
#         self.act_fc1 = nn.Linear(3 * board_width * board_height, board_width * board_height)
#         # state value layers
#         self.val_conv1 = nn.Conv2d(128, 2, kernel_size=1)
#         self.val_fc1 = nn.Linear(2 * board_width * board_height, 64)
#         self.val_fc2 = nn.Linear(64, 1)
#
#     def forward(self, state_input):
#         # common layers
#         x = F.relu(self.conv1(state_input))
#         x = F.relu(self.conv2(x))
#         x = F.relu(self.conv3(x))
#         # action policy layers
#         x_act = F.relu(self.act_conv1(x))
#         x_act = x_act.view(-1, 3 * self.board_width * self.board_height)
#         x_act = F.log_softmax(self.act_fc1(x_act), dim=1)
#         # state value layers
#         x_val = F.relu(self.val_conv1(x))
#         x_val = x_val.view(-1, 2 * self.board_width * self.board_height)
#         x_val = F.relu(self.val_fc1(x_val))
#         x_val = torch.tanh(self.val_fc2(x_val))
#         return x_act, x_val

# 搭建残差块
class ResBlock(nn.Module):

    def __init__(self, num_filters=256):
        super().__init__()
        self.conv1 = nn.Conv2d(in_channels=num_filters, out_channels=num_filters, kernel_size=(3, 3), stride=(1, 1),
                               padding=1)
        self.conv1_bn = nn.BatchNorm2d(num_filters, )
        self.conv1_act = nn.ReLU()
        self.conv2 = nn.Conv2d(in_channels=num_filters, out_channels=num_filters, kernel_size=(3, 3), stride=(1, 1),
                               padding=1)
        self.conv2_bn = nn.BatchNorm2d(num_filters, )
        self.conv2_act = nn.ReLU()

    def forward(self, x):
        y = self.conv1(x)
        y = self.conv1_bn(y)
        y = self.conv1_act(y)
        y = self.conv2(y)
        y = self.conv2_bn(y)
        y = x + y
        return self.conv2_act(y)


# 搭建骨干网络，输入：N, 5, 9, 9 --> N, C, H, W
class Net(nn.Module):

    def __init__(self, num_channels=256, num_res_blocks=7):
        super().__init__()
        # 全局特征
        # self.global_conv = nn.Conv2D(in_channels=9, out_channels=512, kernel_size=(10, 9))
        # self.global_bn = nn.BatchNorm2D(512)
        # 初始化特征
        self.conv_block = nn.Conv2d(in_channels=5, out_channels=num_channels, kernel_size=(3, 3), stride=(1, 1),
                                    padding=1)
        self.conv_block_bn = nn.BatchNorm2d(256)
        self.conv_block_act = nn.ReLU()
        # 残差块抽取特征
        self.res_blocks = nn.ModuleList([ResBlock(num_filters=num_channels) for _ in range(num_res_blocks)])
        # 策略头
        self.policy_conv = nn.Conv2d(in_channels=num_channels, out_channels=16, kernel_size=(1, 1), stride=(1, 1))
        self.policy_bn = nn.BatchNorm2d(16)
        self.policy_act = nn.ReLU()
        self.policy_fc = nn.Linear(16 * 9 * 9, 64)
        # 价值头
        self.value_conv = nn.Conv2d(in_channels=num_channels, out_channels=8, kernel_size=(1, 1), stride=(1, 1))
        self.value_bn = nn.BatchNorm2d(8)
        self.value_act1 = nn.ReLU()
        self.value_fc1 = nn.Linear(8 * 9 * 9, 256)
        self.value_act2 = nn.ReLU()
        self.value_fc2 = nn.Linear(256, 1)


    # 定义前向传播
    def forward(self, x):
        # 公共头
        x = self.conv_block(x)
        x = self.conv_block_bn(x)
        x = self.conv_block_act(x)
        for layer in self.res_blocks:
            x = layer(x)
        # 策略头
        policy = self.policy_conv(x)
        policy = self.policy_bn(policy)
        policy = self.policy_act(policy)
        policy = torch.reshape(policy, [-1, 16 * 9 * 9])
        policy = self.policy_fc(policy)
        policy = F.log_softmax(policy, dim=1)
        # 价值头
        value = self.value_conv(x)
        value = self.value_bn(value)
        value = self.value_act1(value)
        value = torch.reshape(value, [-1, 8 * 9 * 9])
        value = self.value_fc1(value)
        value = self.value_act1(value)
        value = self.value_fc2(value)
        value = F.tanh(value)

        return policy, value


# class PolicyValueNet():
#     """policy-value network """
#
#     def __init__(self, model_file=None, use_gpu=True, device='cuda'):
#         self.device = device
#         # self.use_gpu = use_gpu
#         # self.board_width = board_width
#         # self.board_height = board_height
#         self.l2_const = 2e-3  # coef of l2 penalty
#         # the policy value net module
#         # if self.use_gpu:
#         self.policy_value_net = Net().to(self.device)
#         # else:
#         #     self.policy_value_net = Net()
#         self.optimizer = optim.Adam(self.policy_value_net.parameters(), lr=1e-3, betas=(0.9, 0.999), eps=1e-8,
#                                     weight_decay=self.l2_const)
#
#         if model_file:
#             self.policy_value_net.load_state_dict(torch.load(model_file))
#
#     def policy_value(self, state_batch):
#         """
#         input: a batch of states
#         output: a batch of action probabilities and state values
#         """
#         self.policy_value_net.eval()
#
#         state_batch = torch.tensor(state_batch).to(self.device)
#         log_act_probs, value = self.policy_value_net(state_batch)
#         log_act_probs, value = log_act_probs.cpu(), value.cpu()
#         act_probs = np.exp(log_act_probs.detach().numpy())
#         return act_probs, value.detach().numpy()
#         # else:
#         #     state_batch = torch.tensor(state_batch)
#         #     log_act_probs, value = self.policy_value_net(state_batch)
#         #     act_probs = np.exp(log_act_probs.data.numpy())
#         #     return act_probs, value.data.numpy()
#
#     def policy_value_fn(self, game: GameState):
#         """
#         input: board
#         output: a list of (action, probability) tuples for each available
#         action and the score of the board state
#         """
#         self.policy_value_net.eval()
#         legal_positions = game.legal_moves()
#         current_state = np.ascontiguousarray(encoder_board(game).reshape(-1, 5, 9, 9)).astype('float16')   # .reshape(-1, 3)
#
#         current_state = torch.as_tensor(current_state).to(self.device)
#
#         # log_act_probs, value = self.policy_value_net(current_state)
#
#         with autocast():  #半精度fp16
#             log_act_probs, value = self.policy_value_net(current_state)
#
#         log_act_probs, value = log_act_probs.cpu(), value.cpu()
#
#         act_probs = np.exp(log_act_probs.detach().numpy().astype('float16').flatten())
#
#         act_probs = zip(legal_positions, act_probs[:len(legal_positions)])
#
#         return act_probs, value.detach().numpy()
#
#     def train_step(self, state_batch, mcts_probs, winner_batch, lr=0.002):
#         """perform a training step"""
#         # wrap in Variable
#         self.policy_value_net.train()
#         # 将内部列表转换为torch.Tensor
#         mcts_probs_tensors = [[lst[idx] if idx < len(lst) else 0 for idx in range(81)] for lst in mcts_probs]
#
#         # # 将补齐后的列表转换为PyTorch张量
#         # tensor = torch.tensor(padded_lists, dtype=torch.float32)
#         state_batch = np.array(state_batch)
#
#         state_batch = torch.tensor(state_batch, dtype=torch.float).to(self.device)  # 确保 state_batch 是 numpy 数组
#
#         # mcts_probs = Variable(torch.FloatTensor(mcts_probs).cuda())
#         # 使用pad_sequence补齐张量到最长序列的长度
#         # 将补齐后的列表转换为PyTorch张量
#         mcts_probs = torch.tensor(mcts_probs_tensors, dtype=torch.float32).to(self.device)
#         # mcts_probs = pad_sequence(mcts_probs, batch_first=True, padding_value=0).cuda()
#         winner_batch = torch.tensor(winner_batch, dtype=torch.float).to(self.device)  # 确保 state_batch 是 numpy 数组
#         # else:
#         #     state_batch = torch.tensor(state_batch, dtype=torch.float)
#         #     # mcts_probs = Variable(torch.FloatTensor(mcts_probs))
#         #     # 将补齐后的列表转换为PyTorch张量
#         #     mcts_probs = torch.tensor(mcts_probs_tensors, dtype=torch.float32)
#         #     winner_batch = torch.tensor(winner_batch, dtype=torch.float)
#
#         # zero the parameter gradients
#         self.optimizer.zero_grad()
#         # set learning rate
#         for params in self.optimizer.param_groups:
#             params['lr'] = lr
#
#         # forward
#         log_act_probs, value = self.policy_value_net(state_batch)
#         # define the loss = (z - v)^2 - pi^T * log(p) + c||theta||^2
#         # Note: the L2 penalty is incorporated in optimizer
#         value = torch.reshape(value, shape=[-1])
#         value_loss = F.mse_loss(value, winner_batch)
#
#         policy_loss = -torch.mean(torch.sum(mcts_probs * log_act_probs, 1))
#         loss = value_loss + policy_loss
#         # backward and optimize
#         loss.backward()
#         self.optimizer.step()
#         # calc policy entropy, for monitoring only
#         # 策略的熵
#         with torch.no_grad():
#             entropy = -torch.mean(
#                 torch.sum(torch.exp(log_act_probs) * log_act_probs, 1)
#             )
#
#         # return loss.data[0], entropy.data[0]
#         return loss.detach().cpu().numpy(), entropy.detach().cpu().numpy()
#         # for pytorch version >= 0.5 please use the following line instead.
#         # return loss.item(), entropy.item()
#
#     def save_model(self, model_file):
#         """ save model params to file """
#         # get model params
#         torch.save(self.policy_value_net.state_dict(), model_file)


if __name__ == '__main__':
    net = Net()

    net.load_state_dict(torch.load('./pytorch.pt'))
    net.eval()
    # board = torch.zeros((1, 5, 9, 9))
    # p, v = net(board)

    # print(p)
    # print(v)
    im = torch.zeros(1, 5, 9, 9)

    torch.onnx.export(
        net,  # dynamic=True only compatible with cpu
        im,
        "pytorch_local.onnx",
        verbose=False,
        export_params=True,
        opset_version=get_latest_opset(),
        do_constant_folding=True,  # WARNING: DNN inference with torch>=1.12 may require do_constant_folding=False
        input_names=["board"],
        output_names=["policy", "value"],
        dynamic_axes={"board": {0: "batch_size"}}
    )
