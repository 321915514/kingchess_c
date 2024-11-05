//
// Created by 32191 on 2024/11/1.
//
#include "gamestate.h"
#include "utils.h"
#include "alpha_beta.h"
#include "expert.h"


int run(){
    GameState game;
    int winner = 0;
    GameState::new_game(5,9,game);
    Alpha_beta alpha;
    while (true){
        //print_board(game.board);

        game.is_gameover(winner);
        if(winner == -1) {
            // std::cout << "white win !!" << std::endl;
            return WHITE;
        }
        if(winner == 1) {
            // std::cout << "black win !!" << std::endl;
            return BLACK;
        }
        if(game.player == BLACK) {
            //
            auto moves = Expert_agent::select_move(game);

            auto move = moves.begin()->first;
            auto maxValue = moves.begin()->second;

            for (const auto& pair : moves) {
                if (pair.second > maxValue) {
                    maxValue = pair.second;
                    move = pair.first;
                }
            }



           // auto move = alpha.select_move(game,3);
           // auto score_moves = alpha.score_moves(game,3);
           // for(auto m:score_moves) {
           //     std::cout<<Move::move2str(game.action_2_move(m.first))<<":"<< m.second<<"\t";
           // }
           // std::cout<<std::endl;
           game.apply_move(move,game);
        }else {

            // expert

             auto moves = Expert_agent::select_move(game);
            
             auto move = moves.begin()->first;
             auto maxValue = moves.begin()->second;
            
             for (const auto& pair : moves) {
                 if (pair.second > maxValue) {
                     maxValue = pair.second;
                     move = pair.first;
                 }
             }

            // alpha_beta
            //auto score_moves = alpha.score_moves(game,3);
            //auto move = alpha.select_move(game,3);
        //   for(auto m:score_moves) {
        //        std::cout<<Move::move2str(game.action_2_move(m.first))<<":"<< m.second<<"\t";
         //   }
         //   std::cout<<std::endl;
            // print_move(game.player,move);
            game.apply_move(move,game);
        }


    }
}


int main() {
    int black = 0;
    int white = 0;
    for(int i=0;i<100;i++){
    	auto result = run();
	std::cout<<result<<std::endl;
	if(result == BLACK){
	    black++;
	}
	if(result == WHITE){
	    white++;
	}
    }

    std::cout<<black<<","<<white<<std::endl;
}
