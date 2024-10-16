//
// Created by wangxu on 2023/10/25.
//

#include <chrono>
#include "test1.h"
#include "../fundamental/coordinate.h"
#include "gamestate.h"
#include "agent.h"
int16_t bot_vs_bot(){

    int board_size = 8;
    GameState game;
    GameState::new_game(board_size,game);
    GameState stage2_game;
    while (!game.is_over_stage1(stage2_game)) {
//        print_board(game.board);
        Move move;
        if (game.player == BLACK) {
            //string move_str;
            //cout << "--:";
            //cin >> move_str;
            //Point point = point_from_coords(move_str);
            //Move move = Move::play(point);
            Bot::select_move(game,move);
//            std::cout<<move.m_point<<std::endl;
//            print_move(game.player, move);
            game.apply_move(move,game);
        }else {
            Bot::select_move(game,move);
//            print_move(game.player, move);
            game.apply_move(move,game);
        }

    }

    while (!stage2_game.is_gameover()){
//        print_board(stage2_game.board);
        Move move;
        coord_t square = 0;
        if(stage2_game.player==BLACK){
            int16_t black = stage2_game.board.count_chess(BLACK);
            if(black<Coordinate::m_size){
                // three
//                if(black){
////                    std::cout<<"---------------black  stage3--------------"<<std::endl;
//                    stage2_game.status = stage3;
//                    Bot::select_move(stage2_game, move);
////                    print_move(stage2_game.player, move);
//                    stage2_game.apply_move(move,stage2_game);
//
//                    stage2_game.is_square(move.m_points[move.m_points.size()-1],square);
//                    stage2_game.board.operation_square_simple(stage2_game.player,square);
//                } else{
//                print_board(stage2_game.board);
                return WHITE;
//                }


            }else{
                Bot::select_move(stage2_game, move);
//                print_move(stage2_game.player, move);
                stage2_game.apply_move(move,stage2_game);

                stage2_game.is_square(move.m_points[move.m_points.size()-1],square);
                stage2_game.board.operation_square_simple(stage2_game.player,square);


            }
        }else {
            int16_t white = stage2_game.board.count_chess(WHITE);
            if(white<Coordinate::m_size){
                //
//                if(white){
////                    std::cout<<"---------------white  stage3--------------"<<std::endl;
//                    stage2_game.status = stage3;
//                    Bot::select_move(stage2_game, move);
////                    print_move(stage2_game.player, move);
//                    stage2_game.apply_move(move,stage2_game);
//
//                    stage2_game.is_square(move.m_points[move.m_points.size()-1],square);
//                    stage2_game.board.operation_square_simple(stage2_game.player,square);
//
//                }else{
//                print_board(stage2_game.board);
                return BLACK;
//                }

            }else{
                Bot::select_move(stage2_game, move);
//                print_move(stage2_game.player, move);
                stage2_game.apply_move(move,stage2_game);

                stage2_game.is_square(move.m_points[move.m_points.size()-1],square);
                stage2_game.board.operation_square_simple(stage2_game.player,square);

            }
        }


    }
}






int main(){

    /*
     *             auto start = std::chrono::high_resolution_clock::now();
            bot.select_move(game,move);
            auto end = std::chrono::high_resolution_clock::now();
            // 计算时间间隔
            std::chrono::duration<double> duration = end - start;

            // 输出时间间隔
            std::cout << "time " << duration.count() << "s" << std::endl;
     * */
    auto start = std::chrono::high_resolution_clock::now();
    for(int i=0;i<50000;i++){
//        auto start = std::chrono::high_resolution_clock::now();

         bot_vs_bot();
//        auto end = std::chrono::high_resolution_clock::now();
        // 计算时间间隔
//        std::chrono::duration<double> duration = end - start;

        // 输出时间间隔
//        std::cout << "time " << duration.count() << "s" << std::endl;
    }
    auto end = std::chrono::high_resolution_clock::now();
    // 计算时间间隔
    std::chrono::duration<double> duration = end - start;

    // 输出时间间隔
    std::cout << "sum time " << duration.count() << "s" << std::endl;
}
