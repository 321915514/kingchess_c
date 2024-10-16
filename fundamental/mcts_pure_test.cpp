//
// Created by philxling on 2024/9/8.
//


// TicMcts.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include "coordinate.h"
#include "board.h"
#include <map>
#include "agent.h"
#include "chrono"
#include <iostream>
#include "utils.h"
#include "../net//mcts_pure.h"
using namespace std;

int main()
{
    MCTS_Pure bot(64,5,10000);

    GameState game;
    GameState::new_game(5,9,game);

    memset(game.board.grid,0,sizeof(game.board.grid));

    game.board.grid[2] = -1;
    game.board.grid[3] = -1;
    game.board.grid[4] = -1;
    game.board.grid[5] = 1;
    game.board.grid[6] = -1;
    game.board.grid[11] = 1;
    game.board.grid[12] = -1;
    game.board.grid[13] = -1;
    game.board.grid[14] = -1;
    game.board.grid[15] = -1;
    game.board.grid[20] = -1;
    game.board.grid[21] = -1;
    game.board.grid[22] = -1;
    game.board.grid[23] = -1;
    game.board.grid[24] = -1;
    game.board.grid[29] = -1;
    game.board.grid[30] = -1;
    game.board.grid[31] = -1;
    game.board.grid[32] = -1;
    game.board.grid[33] = -1;
    game.board.grid[34] = -1;
    game.board.grid[41] = -1;

    while (true) {
        print_board(game.board);
        Move move;
        int winner=0;
        game.is_gameover(winner);
        if (winner != 0) {
            cout << "winner:" << winner << endl;
            break;
        }
        auto start = std::chrono::high_resolution_clock::now();
        auto game_copy = GameState(game);
        auto result = bot.get_action_probs(&game_copy,false,5);
        bot.update_with_move(-1);
//        for(auto p:result){
//            std::cout<<p<<",";
//        }
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











