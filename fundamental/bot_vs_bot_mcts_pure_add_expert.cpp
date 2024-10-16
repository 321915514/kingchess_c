//
// Created by 32191 on 2024/9/26.
//

#include <cassert>

#include "expert.h"
#include "gamestate.h"

#include "../net/mcts_add_expert.h"

#include "coordinate.h"
#include "board.h"
#include "chrono"
#include <iostream>
#include "utils.h"
#include <cassert>
#include <fstream>
#include "train.pb.h"


void write_proto_file(std::vector<std::tuple<std::array<float,81*5>,std::vector<float>,int>>& play_data){

    TripletArray triplet_array;
    // 在此处填充你的 TripletArray 实例数据

    for (auto i=0;i<play_data.size();i++) {
        Triplet triplet = Triplet();

        for (auto j = 0; j < 81*5; j++) {
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




std::vector<std::tuple<std::array<float,81*5>,std::vector<float>,int>> symmetry(std::array<float,81*5>& state, std::vector<float>& move_prob,const int& current_player){
    std::vector<std::tuple<std::array<float,81*5>,std::vector<float>,int>> results;
    for(size_t code=0;code<8;++code){
        for (int row = 0; row < 5; ++row) {
            if(row!=2){
                float temp[81]={0};
                for (int col = 0; col < 81; ++col) {

                    temp[col]  = state[ row*81 + Coordinate::m_symmetry[(81*row+col)%81][code]]; // 返回
                }

                for(auto index=row*81;index<(row*81)+81;++index){
                    state[index] = temp[index%81];
                }

            }
        }
        results.emplace_back(state,move_prob,current_player);
    }
    return results;
}





std::vector<std::tuple<std::array<float,81*5>,std::vector<float>,int>> get_equi_data(std::vector<std::array<float,81*5>> states,  std::vector<std::vector<float>> move_probs, std::vector<int> current_players){

    assert(states.size() == move_probs.size() == current_players.size());

    std::vector<std::tuple<std::array<float,81*5>,std::vector<float>,int>> play_data;

    for(auto i=0;i<states.size();++i){
        //
        auto symmetries = symmetry(states[i],move_probs[i],current_players[i]); // return std::vector<std::tuple<float*,std::vector<std::pair<std::string,float>>,int>>

        play_data.insert(play_data.end(),symmetries.begin(),symmetries.end());
    }
    return play_data;
}






void self_play(){
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<std::array<float,81*5>> states;
    std::vector<std::vector<float>> move_probs;
    std::vector<int> current_players;
    float grid[81*5]={0};
    std::array<float,81*5> arr{0.0f};
    MCTS bot(64,5,10000,0.01);

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
            auto play_data = get_equi_data(states,move_probs,winner_z);

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
        auto result = bot.get_action_probs(&game_copy,false,5);

        // 存数据
        std::vector<float> move_prob;
        for(size_t i =0;i<1125;i++){
            move_prob.push_back(result[i]); // move2str
        }

        auto game_encoder = std::make_shared<GameState>(game);

        memset(grid,0,sizeof(grid));

        encoder_data(game_encoder,grid);

        memcpy(arr.data(),grid,sizeof(grid));

        states.push_back(arr);

        move_probs.emplace_back(move_prob);
        current_players.push_back(game.player);

        // 存数据结束

        bot.update_with_move(-1);
        // for(auto p:result){
        //     std::cout<<p<<",";
        // }
        auto max_iter = std::max_element(result.begin(), result.end());
        std::cout<<*max_iter<<std::endl;
        auto index = std::distance(result.begin(), max_iter);
        move = game.action_2_move(index);
        // 记录结束时间
        auto end = std::chrono::high_resolution_clock::now();

        // 计算时间差
        std::chrono::duration<double> duration = end - start;
        // 输出耗时
        std::cout << "Time taken: " << duration.count() << " seconds" << std::endl;

        print_move(game.player, move);
        game.apply_move(move,game);

    }
}




// int play() {
//     GameState game;
//     GameState::new_game(5, 9, game);
//     MCTS mcts(16, 5, 1600, 0.3);
//     Move m;
//     int winner = 0;
//     while (true) {
//         // print_board(game.board);
//         game.is_gameover(winner);
//         if (winner != 0) {
//             // if (winner == WHITE) {
//             //     std::cout << "white" << std::endl;
//             // }
//             // if (winner == BLACK) {
//             //     std::cout << "black" << std::endl;
//             // }
//
//             return winner;
//
//         }
//
//         // if(game.player == BLACK) {
//         //     Bot::select_move(game,m);
//         //     game.apply_move(m,game);
//         // }else {
//         // auto moves = Expert_agent::select_move(game);
//         // for(auto item:moves) {
//         //     std::cout<<Move::move2str(item.first) <<":"<<item.second<<"\t";
//         // }
//         // std::cout<<std::endl;
//
//         // auto maxElement = std::max_element(moves.begin(), moves.end(),
//         //                             [](const auto& a, const auto& b) {
//         //                                 return a.second < b.second;
//         //                             });
//         // print_move(maxElement->first);
//         auto game_copy = GameState(game);
//         auto result = mcts.get_action_probs(&game_copy, false, 5);
//         mcts.update_with_move(-1);
//         auto max_iter = std::max_element(result.begin(), result.end());
//         //        std::cout<<*max_iter<<std::endl;
//         auto index = std::distance(result.begin(), max_iter);
//         m = game.action_2_move(index);
//         game.apply_move(m, game);
//     }
// }


int main() {


    while (true) {
        self_play();
    }

    // int black =0;
    // int white = 0;
    // for(size_t i=0;i<100;i++) {
    //     auto winner = play();
    //     if (winner == WHITE) {
    //         // std::cout << "white" << std::endl;
    //         white++;
    //     }
    //     if (winner == BLACK) {
    //         // std::cout << "black" << std::endl;
    //         black++;
    //     }
    // }
    //
    // std::cout<<"black:"<<black<<"white:"<<white<<std::endl;


}
