//
// Created by 32191 on 2024/9/30.
//
#include "gamestate.h"
#include "utils.h"
#include "alpha_beta.h"

#include <cassert>
#include <chrono>
#include <future>

#include "agent.h"
#include "train.pb.h"
#include <fstream>


void write_proto_file(std::vector<std::tuple<std::array<float,45*5>,std::vector<float>,int>>& play_data){

    TripletArray triplet_array;
    // 在此处填充你的 TripletArray 实例数据

    for (auto i=0;i<play_data.size();i++) {
        Triplet triplet = Triplet();

        for (auto j = 0; j < 45*5; j++) {
            triplet.add_array(static_cast<int32_t>(std::get<0>(play_data[i])[j]));
        }


        for (const auto& item : std::get<1>(play_data[i])){

            triplet.add_dictionary(static_cast<float>(item));

        }
        triplet.set_value(std::get<2>(play_data[i]));
        *triplet_array.add_triplets() = triplet;

    }

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    char filename[80];
    //LOG(ERROR)<<"get filename";
    sprintf(filename, "file_%d-%02d-%02d-%02d-%02d-%02d.game",
            tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
            tm.tm_hour, tm.tm_min, tm.tm_sec);
    std::cout<<"Current file_name:"<<filename;

    std::string file(filename);

    std::string filename1;

    filename1 = "/home/dev/dataset_cpp/"+file;

    // 将消息序列化到文件
    std::fstream output(filename1, std::ios::out | std::ios::binary | std::ios::trunc);
    if (!triplet_array.SerializeToOstream(&output)) {
        std::cout << "Failed to write triplet array." << std::endl;
        return;
    }
    std::cout <<filename<< " write." << std::endl;
}

// std::vector<std::tuple<std::array<float,81*5>,std::vector<float>,int>> get_equi_data(std::vector<std::array<float,81*5>> states,  std::vector<std::vector<float>> move_probs, std::vector<int> current_players){
//
//     assert(states.size() == move_probs.size() == current_players.size());
//
//     std::vector<std::tuple<std::array<float,81*5>,std::vector<float>,int>> play_data;
//
//     for(auto i=0;i<states.size();++i){
//         //
//         auto symmetries = symmetry(states[i],move_probs[i],current_players[i]); // return std::vector<std::tuple<float*,std::vector<std::pair<std::string,float>>,int>>
//
//         play_data.insert(play_data.end(),symmetries.begin(),symmetries.end());
//     }
//     return play_data;
// }





void self_play(){
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<std::array<float,45*5>> states;
    std::vector<std::vector<float>> move_probs;
    std::vector<int> current_players;
    float grid[45*5]={0};
    std::array<float,45*5> arr{0.0f};
    Alpha_beta alpha;
    // MCTS bot(64,5,10000,0.01);

    GameState game;
    GameState::new_game(5,9,game);
    while (true) {
        // print_board(game.board);
        Move move;
        int winner=0;
        game.is_gameover(winner);
        if (winner != 0) {

            // 结束，修改winner
            std::vector<int> winner_z(current_players.size(),0);
            if(winner != -1){
                for(size_t i=0;i<current_players.size();i++){
                    if(current_players[i] == winner){
                        winner_z[i] = 1.0;
                    }else{
                        winner_z[i] = -1.0;
                    }
                }
            }

            // 增强数据

            std::vector<std::tuple<std::array<float,45*5>,std::vector<float>,int>> play_data;
            for(int i=0;i<states.size();i++) {
                play_data.emplace_back(states[i],move_probs[i],winner_z[i]);
            }

            write_proto_file(play_data);

            std::cout<<"game state length:"<<states.size();

            auto end = std::chrono::high_resolution_clock::now();
            // 计算时间间隔
            std::chrono::duration<double> duration = end - start;
            // 输出时间间隔
            std::cout << "speed time " << duration.count() << "s";

            std::cout << "winner:" << winner << std::endl;
            break;
        }
        auto start = std::chrono::high_resolution_clock::now();
        auto game_copy = GameState(game);

        // auto result = bot.get_action_probs(&game_copy,false,5);
        move = alpha.select_move(game,3);

        auto action = game.move_2_action(move);

        // 存数据
        std::vector<float> move_prob(1125,0);

        move_prob[action] = 1; //


        auto game_encoder = std::make_shared<GameState>(game);

        memset(grid,0,sizeof(grid));

        encoder_data_45(game_encoder,grid);

        memcpy(arr.data(),grid,sizeof(grid));

        states.push_back(arr);

        move_probs.emplace_back(move_prob);
        current_players.push_back(game.player);

        // 存数据结束

        // bot.update_with_move(-1);
        // for(auto p:result){
        //     std::cout<<p<<",";
        // }
        // auto max_iter = std::max_element(result.begin(), result.end());
        // std::cout<<*max_iter<<std::endl;
        // auto index = std::distance(result.begin(), max_iter);
        // move = game.action_2_move(index);
        // 记录结束时间
        auto end = std::chrono::high_resolution_clock::now();

        // 计算时间差
        std::chrono::duration<double> duration = end - start;
        // 输出耗时
        std::cout << "Time taken: " << duration.count() << " seconds" << std::endl;

        // print_move(game.player, move);
        game.apply_move(move,game);

    }
}




// int run(){
//     GameState game;
//     int winner = 0;
//     GameState::new_game(5,9,game);
//     Alpha_beta alpha;
//     while (true){
//         print_board(game.board);
//         //        if(game.eat_chess()>=11){
//         //            std::cout<<"black win !!"<<std::endl;
//         //            break;
//         //        }
//         // is game_over
//         game.is_gameover(winner);
//         if(winner == -1) {
//             // std::cout << "white win !!" << std::endl;
//             return WHITE;
//         }
//         if(winner == 1) {
//             // std::cout << "black win !!" << std::endl;
//             return BLACK;
//         }
//         if(game.player == BLACK) {
//             // expert
//             auto moves = Expert_agent::select_move(game);
//
//             auto maxElement = std::max_element(moves.begin(), moves.end(),
//                                         [](const auto& a, const auto& b) {
//                                             return a.second < b.second;
//                                         });
//             game.apply_move(maxElement->first,game);
//
//             // expert end
//
//
//
//
//
//
//
//             // auto move = alpha.select_move(game,5);
//             // // print_move(game.player,move);
//             // game.apply_move(move,game);
//         }else {
//             // auto moves = Expert_agent::select_move(game);
//             //
//             // auto maxElement = std::max_element(moves.begin(), moves.end(),
//             //                             [](const auto& a, const auto& b) {
//             //                                 return a.second < b.second;
//             //                             });
//             // game.apply_move(maxElement->first,game);
//
//
//             // alpha_beta
//             auto move = alpha.select_move(game,3);
//             // print_move(game.player,move);
//             game.apply_move(move,game);
//
//
//             // random
//             // Move move;
//             // Bot::select_move(game,move);
//             // // print_move(game.player,move);
//             // game.apply_move(move,game);
//
//
//             // alpha white
//             // auto move = alpha.select_move(game,5);
//             // // print_move(game.player,move);
//             // game.apply_move(move,game);
//         }
//
//
//     }
// }

// void simpleProgressBar(int progress, std::chrono::duration<double> duration) {
//     const int bar_length = 50;
//     int pos = static_cast<int>(bar_length * (progress / 100.0));
//     std::cout << "[";
//     for (int i = 0; i < bar_length; ++i) {
//         if (i < pos)
//             std::cout << "=";
//         else
//             std::cout << " ";
//     }
//     std::cout << "] " << progress << "%" << " Time taken: " << duration.count() << " seconds" << std::endl;
//     // std::cout << "Time taken: " << duration.count() << " seconds" << std::endl;
// }


int main() {
    for(int i=0;i<10000;i++) {
        self_play();
    }

    // int black = 0;
    // int white =0;
    // std::unique_ptr<ThreadPool> thread_pool;
    //
    // std::vector<std::future<int>> futures;
    //
    // for (unsigned int i = 0; i < 10; i++) {
    //     auto future = thread_pool->commit(run);
    //     futures.emplace_back(std::move(future));
    // }
    // for (unsigned int i = 0; i < futures.size(); i++) {
    //     futures[i].wait();
    //     int result = futures[i].get();
    //     if(result == BLACK) {
    //         black++;
    //     }
    //     if(result == WHITE) {
    //         white++;
    //     }
    // }

    // std::vector<std::future<int>> futures;
    // for (int i = 0; i < 5; ++i) {
    //     futures.push_back(std::async(std::launch::async, run));
    // }
    //
    // for (size_t i = 0; i < futures.size(); ++i) {
    //     auto result = futures[i].get();
    //     if(result == BLACK) {
    //         black++;
    //     }
    //     if(result == WHITE) {
    //         white++;
    //     }
    // }





    //
    //
    // for(int i=0;i<100;i++) {
    //
    //     auto start = std::chrono::high_resolution_clock::now();
    //
    //     auto result = run();
    //     if(result == BLACK) {
    //         black++;
    //     }
    //     if(result == WHITE) {
    //         white++;
    //     }
    //     auto end = std::chrono::high_resolution_clock::now();
    //
    //     // 计算时间差
    //     std::chrono::duration<double> duration = end - start;
    //
    //     simpleProgressBar(i,duration);
    //
    //     // std::cout << "Time taken: " << duration.count() << " seconds" << std::endl;
    //
    // }
    // std::cout<<"black:"<<black<<"white:"<<white<<std::endl;   // black:99white:1 // black:79white:21

}