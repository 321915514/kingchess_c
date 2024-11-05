//
// Created by wangxu on 2023/8/7.
//

#include "agent.h"

void Bot::select_move(GameState &game, Move &move) {
    if (game.player == BLACK) {
        // is black
//        std::vector<coord_t> black_chess;
//        for(coord_t coord:Coordinates){
//            if(game.board.grid[coord] == BLACK){
//                black_chess.push_back(coord);
//            }
//        }
        //

//        for(auto coord:black_chess){
//            std::cout<<coord<<"\t";
//        }
//        std::cout<<std::endl;
        std::vector<Move> moves;

        game.legal_moves(moves);

//        for(const auto& it:moves){
//            print_move(it);
//        }
//        std::cout<<std::endl;


        if (moves.empty()) {
            move.m_point = -100;
            move.m_point_ = -100;
            return;
        }
        auto index = get_random_index(moves.size());
        move = moves[index];
    } else if (game.player == WHITE) {
        // is white
//        if(game.play_out <= 32){
//            std::vector<Move> moves;
//            for(auto coord:Coordinates){
//                if(Coordinate::Legal_coord(coord) && Coordinate::OnBoard(coord) && game.board.grid[coord] == 0){
//                    moves.push_back(Move(coord));
//                }
//            }
//            // < 32
//            // 打印move
////            for(const auto& it:moves){
////                print_move(it);
////            }
//
//            auto index = get_random_index(moves.size());
//            move = moves[index];
//        }else{
//            std::vector<coord_t> white_chess;
//            for(coord_t coord:Coordinates){
//                if(game.board.grid[coord] == WHITE){
//                    white_chess.push_back(coord);
//                }
//            }
//            std::vector<Move> moves;
//
//
//
//            game.get_legal_moves(white_chess,moves);

//            for(const auto& it:moves){
//                print_move(it);
//            }
//            std::cout<<std::endl;
        std::vector<Move> moves;

        game.legal_moves(moves);

        auto index = get_random_index(moves.size());
	
        move = moves[index];
    }
}

