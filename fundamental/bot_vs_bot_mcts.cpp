//
// Created by wangxu on 2023/8/7.
//

// TicMcts.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include "coordinate.h"
#include "board.h"
#include <map>
//#include "agent.h"
#include "chrono"
#include <iostream>
#include "utils.h"
#include "../mcts/mcts.h"
using namespace std;

int main()
{

    Mcts_bot bot(1000);
    GameState game;
    GameState::new_game(5,9,game);

    while (true) {
        print_board(game.board);
        Move move;

        if(game.eat_chess()>=11) {
            std::cout<<"black win !!"<<std::endl;
            break;
        }
        if (game.player == BLACK) {
            //string move_str;
            //cout << "--:";
            //cin >> move_str;
            //Point point = point_from_coords(move_str);
            //Move move = Move::play(point);
            // 记录开始时间
            auto start = std::chrono::high_resolution_clock::now();

            bot.select_move(game,move);

            // 记录结束时间
            auto end = std::chrono::high_resolution_clock::now();

            // 计算时间差
            std::chrono::duration<double> duration = end - start;

            // 输出耗时
            std::cout << "Time taken: " << duration.count() << " seconds" << std::endl;

            if(move.m_point == -100 && move.m_point_ == -100){
                std::cout<<"white win !!"<<std::endl;
                break;
            }

//            std::cout<<move.m_point<<std::endl;
            print_move(game.player, move);
            game.apply_move(move,game);
        }else {

            // 记录开始时间
            auto start = std::chrono::high_resolution_clock::now();

            bot.select_move(game,move);

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
}











