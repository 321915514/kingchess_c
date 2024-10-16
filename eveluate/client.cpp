#include <iostream>
#include <string>
#include <thread>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "json.hpp"
#include "kingchess_c/fundamental/gamestate.h"
#include "kingchess_c/net/mcts_45.h"
//#include "kingchess_c/fundamental/expert.h"
#include "kingchess_c/net/deep_model_45.h"


using json = nlohmann::json;

// 定义一个函数来处理客户端连接
void handle_client(int client_socket) {
    // 定义一个缓冲区来存储从客户端接收的数据
    char buffer[512] = {0};

    // 接收客户端发来的数据
    ssize_t bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received > 0) {
        // 解析接收到的数据
        std::string received_data(buffer);

        // 假设数据是 JSON 格式的，尝试解析
	std::cout<<received_data<<std::endl;
        try {
            // 这里使用一个假定的函数来解析 JSON 数据
            json json_data = json::parse(received_data);
	    //std::cout<<"parse"<<std::endl;
	    //std::cout<<json_data;
	    //std::cout<<"parse end";
            // 将棋盘变为game对象，并调用get_action
            GameState game;
            GameState::new_game(5,9,game);
            const auto& board = json_data["board"];
	    for(size_t i=0;i<board.size();i++){
	    	game.board.grid[i] = static_cast<int16_t>(board[i]);
	    }
            game.player = json_data["player"];
            game.play_out = json_data["play_out"];
            // 构建deepmodel
            deep_model model("/home/test4/new_kingchess/net/model/current.engine");

            auto game_copy = GameState(game);
            //auto mcts = MCTS_Pure(64,5,100000);
            auto mcts = MCTS(&model, 16, 5, 100, 0.3);
            auto res = mcts.get_action_probs(&game_copy, false, 1e-3);
            auto it = std::find(res.begin(), res.end(), 1);
            size_t index = std::distance(res.begin(), it);
	    
	    //auto moves = Expert_agent::select_move(game_copy);
	    //auto maxElement = std::max_element(moves.begin(),moves.end(),[](const auto& a,const auto& b){return a.second<b.second;});

	    //auto index = game_copy.move_2_action(maxElement->first);

            json j;
            j["action"] = index;
            std::string response = j.dump();
            // 发送
            send(client_socket, response.c_str(), response.size(), 0);
        }
        catch (const std::exception& e) {
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
    server_address.sin_port = htons(8888); // 监听端口
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

    std::cout << "Server listening on port 8888" << std::endl;

    // 主循环，持续监听客户端连接
    while (true) {
        // 接受客户端连接
        int client_socket;
        socklen_t client_length = sizeof(client_socket);
        client_socket = accept(server_socket, nullptr, &client_length);

        if (client_socket != -1) {
            std::cout << "Connected by client" << std::endl;
            // 创建新线程来处理客户端连接
            std::thread client_thread(handle_client, client_socket);
            // 设置线程为分离模式，这样主线程结束时线程也会自动结束
            client_thread.detach();
        }
    }

    // 关闭服务器套接字
    close(server_socket);
    return 0;
}
