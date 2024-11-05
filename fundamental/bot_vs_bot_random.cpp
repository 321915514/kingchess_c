//
// Created by wangxu on 2024/1/25.
//

#include <iostream>
#include <cstdint>
#include <deque>
#include "neighbor.h"
#include "coordinate.h"
#include "gamestate.h"
#include "utils.h"
#include <chrono>
#include "agent.h"
#include <iomanip>


//void simple(){
//    GameState game;
//    GameState::new_game(5,9,game);
//    while (true){
////        print_board(game.board);
//        if(game.eat_chess()>=11){
////            std::cout<<"black win !!"<<std::endl;
//            break;
//        }
//        Move move;
//        if(game.player == BLACK){
//            Bot::select_move(game,move);
//            if(move.m_point == -100 && move.m_point_ == -100){
////                std::cout<<"white win !!"<<std::endl;
//                break;
//            }
////            print_move(game.player,move);
//        } else{
//            Bot::select_move(game,move);
////            print_move(game.player,move);
//        }
//        game.apply_move(move,game);
//
//    }
//}


void run(){
    GameState game;
    int winner = 0;
    GameState::new_game(5,9,game);
    while (true){
//
        print_board(game.board);
//        if(game.eat_chess()>=11){
//            std::cout<<"black win !!"<<std::endl;
//            break;
//        }
        // is game_over
        game.is_gameover(winner);
        if(winner == -1) {
//            std::cout << "white win !!" << std::endl;
            std::cout<<game.moves.size()<<std::endl;
            std::cout<<game.play_out<<std::endl;
            break;
        }else if(winner == 1) {
//            std::cout << "black win !!" << std::endl;
	     
            std::cout<<game.moves.size()<<std::endl;
            std::cout<<game.play_out<<std::endl;
            break;
        }
        Move move;
//        if(game.player == BLACK){

//        auto start = std::chrono::high_resolution_clock::now();

        Bot::select_move(game,move);
//        auto end = std::chrono::high_resolution_clock::now();
        // 计算时间间隔
//        std::chrono::duration<double> duration = end - start;
        // 输出时间间隔
//        std::cout << "time " << duration.count() << "s" << std::endl;
//        print_move(game.player,move);
//        }else{
//            Bot::select_move(game,move);
//            print_move(game.player,move);
//        }
        game.apply_move(move,game);
    }
}




int main(){

    auto start = std::chrono::high_resolution_clock::now();
    for(int16_t i =0;i<1;i++){
////        simple();
       run();
    }
//    run();
    auto end = std::chrono::high_resolution_clock::now();
    // 计算时间间隔
    std::chrono::duration<double> duration = end - start;
    // 输出时间间隔
    std::cout << "time " << duration.count() << "s" << std::endl;

}
