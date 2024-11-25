#include "kingchess_c/fundamental/alpha_beta.h"
#include "kingchess_c/fundamental/gamestate.h"
#include "kingchess_c/net/mcts_45_expert.h"
#include "kingchess_c/fundamental/expert.h"
#include "kingchess_c/net/deep_model_45.h"


int run(){
    GameState game;
    int winner = 0;
    GameState::new_game(5,9,game);
    Alpha_beta alpha;
    deep_model model("/home/test4/kingchess_remote/kingchess_c/model/70000.engine");
    auto mcts = MCTS(&model,1,16,1200,0.3);
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
            //expert
            //auto moves = Expert_agent::select_move(game);

            //auto move = moves.begin()->first;
            //auto maxValue = moves.begin()->second;

            //for (const auto& pair : moves) {
            //    if (pair.second > maxValue) {
            //        maxValue = pair.second;
            //        move = pair.first;
            //    }
            //}
            // game.apply_move(move,game);
	    // expert end
	    
	    // model
	    //auto game_copy = GameState(game);
	    //auto res = mcts.get_action_probs(&game_copy,false,1e-3);
            //mcts.update_with_move(-1);
	    //auto it = std::find(res.begin(),res.end(),1);
	    //size_t index = std::distance(res.begin(),it);
            //game.apply_move(game.action_2_move(index), game);
	    // model end

	   // alpha 
           auto move = alpha.select_move(game,2);
           // auto score_moves = alpha.score_moves(game,3);
           // for(auto m:score_moves) {
           //     std::cout<<Move::move2str(game.action_2_move(m.first))<<":"<< m.second<<"\t";
           // }
           // std::cout<<std::endl;
           game.apply_move(move,game);
	   // alpha end
        }else {
            //expert
            //auto moves = Expert_agent::select_move(game);

            //auto move = moves.begin()->first;
            //auto maxValue = moves.begin()->second;

            //for (const auto& pair : moves) {
            //    if (pair.second > maxValue) {
            //        maxValue = pair.second;
            //        move = pair.first;
            //    }
            //}
            // game.apply_move(move,game);
	    // expert end
	    
	    // model
	    auto game_copy = GameState(game);
	    auto res = mcts.get_action_probs(&game_copy,false,1e-3);
            mcts.update_with_move(-1);
	    auto it = std::find(res.begin(),res.end(),1);
	    size_t index = std::distance(res.begin(),it);
	    //std::cout<<Move::move2str(game.action_2_move(index))<<std::endl;
	    game.apply_move(game.action_2_move(index), game);
	    // model end

	   // alpha 
           //auto move = alpha.select_move(game,3);
           //auto score_moves = alpha.score_moves(game,3);
           //for(auto m:score_moves) {
           //     std::cout<<Move::move2str(game.action_2_move(m.first))<<":"<< m.second<<"\t";
           // }
           // std::cout<<std::endl;
           // game.apply_move(move,game);
	   // alpha end

        }


    }
}


int main() {
    int black = 0;
    int white = 0;
    for(int i=0;i<100;i++){
    	auto result = run();
	std::cout<<i<<std::endl;
	if(result == BLACK){
	    black++;
	}
	if(result == WHITE){
	    white++;
	}
    }

    std::cout<<"alpha black:"<<black<<","<<" model white:"<<white<<std::endl;
}
