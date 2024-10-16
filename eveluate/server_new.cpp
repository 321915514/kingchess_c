#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "json.hpp"
#include "kingchess_c/fundamental/gamestate.h"
#include "kingchess_c/net/mcts.h"
#include "kingchess_c/net/deep_model.h"
#include <sys/inotify.h>
#include <fcntl.h>
#define EVENT_SIZE(X) ((X) & ~0x7FL)
using json = nlohmann::json;

// 单例模式管理 model 和 MCTS 对象
class ModelManager {
public:
    static ModelManager& getInstance() {
        static ModelManager instance;
        return instance;
    }

    deep_model& getModel() {
        std::lock_guard<std::mutex> lock(_mutex);
        if (!_model) {
            _model = std::make_unique<deep_model>("/home/test4/new_kingchess/net/model/current.engine");
        }
        return *_model;
    }

    MCTS& getMCTS() {
        std::lock_guard<std::mutex> lock(_mutex);
        if (!_mcts) {
            _mcts = std::make_unique<MCTS>(&getModel(), 64, 3, 1200, 0.3);
        }
        return *_mcts;
    }

    void reloadModel(const std::string& path) {
        std::lock_guard<std::mutex> lock(_mutex);
        _model = std::make_unique<deep_model>(path);
        _mcts = std::make_unique<MCTS>(&getModel(), 64, 3, 1200, 0.3);
    }

private:
    ModelManager() {}
    ~ModelManager() {}

    std::unique_ptr<deep_model> _model;
    std::unique_ptr<MCTS> _mcts;
    std::mutex _mutex;
};
// 文件变化监控函数
void monitor_file_changes(const std::string& path) {
    int fd = inotify_init();
    if (fd == -1) {
        std::cerr << "Failed to initialize inotify" << std::endl;
        return;
    }

    int wd = inotify_add_watch(fd, path.c_str(), IN_MODIFY);
    if (wd == -1) {
        std::cerr << "Failed to add watch for file: " << path << std::endl;
        return;
    }

    while (true) {
        char buffer[1024*40];
        ssize_t len = read(fd, buffer, sizeof(buffer));
        if (len == -1) {
            std::cerr << "Failed to read from inotify" << std::endl;
            break;
        }

        for (ssize_t i = 0; i < len; i += sizeof(inotify_event) + EVENT_SIZE(buffer[i])) {
            struct inotify_event* event = (struct inotify_event*)&buffer[i];
            if (event->wd == wd && event->mask & IN_MODIFY) {
                std::cout << "File modified: " << path << std::endl;
                ModelManager::getInstance().reloadModel(path);
            }
        }
    }

    inotify_rm_watch(fd, wd);
    close(fd);
}

// 定义一个函数来处理客户端连接
void handle_client(int client_socket) {
    // 定义一个缓冲区来存储从客户端接收的数据
    char buffer[512] = {0};

    // 接收客户端发来的数据
    ssize_t bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received > 0) {
        // 解析接收到的数据
        std::string received_data(buffer, bytes_received);

        try {
            // 这里使用一个假定的函数来解析 JSON 数据
            json json_data = json::parse(received_data);

            // 构建 GameState 对象
            GameState game;
            GameState::new_game(5, 9, game);
            const auto& board = json_data["board"];
            for (size_t i = 0; i < board.size(); ++i) {
                game.board.grid[i] = static_cast<int16_t>(board[i]);
            }
            game.player = json_data["player"];
            game.play_out = json_data["play_out"];

            // 获取 MCTS 对象
            auto& mcts = ModelManager::getInstance().getMCTS();

            // 获得动作概率
            auto res = mcts.get_action_probs(&game, false, 1e-3);
            auto it = std::find(res.begin(), res.end(), 1);
            size_t index = std::distance(res.begin(), it);

            // 准备响应
            json j;
            j["action"] = index;
            std::string response = j.dump();

            // 发送响应
            send(client_socket, response.c_str(), response.size(), 0);
        } catch (const std::exception& e) {
            // 如果解析 JSON 数据失败，则发送错误响应
            std::string response = "Error parsing JSON: " + std::string(e.what());
            send(client_socket, response.c_str(), response.size(), 0);
        }
    }

    // 关闭客户端套接字
    close(client_socket);
}

int main() {
    // 创建服务器套接字
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        std::cerr << "Failed to create socket" << std::endl;
        return 1;
    }

    // 设置服务器地址结构
    sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(8999); // 监听端口
    server_address.sin_addr.s_addr = INADDR_ANY; // 监听所有地址

    // 绑定服务器套接字到地址
    if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
        std::cerr << "Failed to bind socket" << std::endl;
        return 1;
    }

    // 开始监听连接
    if (listen(server_socket, 5) == -1) {
        std::cerr << "Failed to listen on socket" << std::endl;
        return 1;
    }

    std::cout << "Server listening on port 65432" << std::endl;

    // 启动文件监控线程
    std::thread file_monitor_thread(monitor_file_changes, "/home/test4/new_kingchess/net/model/current.engine");

    // 主循环，持续监听客户端连接
    while (true) {
        // 接受客户端连接
        int client_socket;
        socklen_t client_length = sizeof(sockaddr_in);
        client_socket = accept(server_socket, nullptr, &client_length);

        if (client_socket != -1) {
            std::cout << "Connected by client" << std::endl;
            // 创建新线程来处理客户端连接
            std::thread client_thread(handle_client, client_socket);
            // 设置线程为分离模式，这样主线程结束时线程也会自动结束
            client_thread.detach();
        }
    }

    // 等待文件监控线程结束
    file_monitor_thread.join();

    // 关闭服务器套接字
    close(server_socket);
    return 0;
}
