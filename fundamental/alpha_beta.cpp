//
// Created by 32191 on 2024/9/30.
//
#include "alpha_beta.h"


double Alpha_beta::evaluation_state(GameState &game_state) {
    double value =0;
    auto score_moves = Expert_agent::select_move(game_state);
    if(score_moves.size()) {
        for(auto move_map:score_moves) {
            if(move_map.second>10) {
                value+=move_map.second;
            }
        }
        return value;
    }
    return -9999;
}



double Alpha_beta::minmax(GameState &game, double alpha, double beta, int depth, bool is_max_depth) {
    auto game_copy = GameState(game);
    int winner_gamecopy = 0;
    game_copy.is_gameover(winner_gamecopy);
    if(depth==0 || winner_gamecopy!=0) {
        return evaluation_state(game_copy);
    }

    if(is_max_depth) {
        double value = -9999;
        std::vector<Move> moves;
        game_copy.legal_moves(moves);
        for(auto m:moves) {
            auto game_copy_1 = GameState(game_copy);
            game_copy_1.apply_move(m,game_copy_1);
            value = std::max(value,minmax(game_copy_1,alpha,beta,depth-1,false));
            alpha = std::max(value,alpha);
            if(alpha>=beta) {
                break;
            }
        }
        return value;
    }else {
        double value = 9999;
        std::vector<Move> moves;
        game_copy.legal_moves(moves);
        for(auto m:moves) {
            auto game_copy_1 = GameState(game_copy);
            game_copy_1.apply_move(m,game_copy_1);
            value = std::min(value,minmax(game_copy_1,alpha,beta,depth-1,true));
            beta = std::min(value,beta);
            if(alpha>=beta) {
                break;
            }
        }
        return value;
    }
}

Move Alpha_beta::select_move(GameState &game, int depth) {
    auto game_copy = GameState(game);
    bool is_max_state = game_copy.player == BLACK;
    auto best_value = is_max_state? -9999 : 9999;
    std::vector<Move> top_moves;
    auto score_moves =  Expert_agent::select_move(game_copy);
    auto best_move = Move();
    for(auto m:score_moves) {
        if(m.second>10) {
            top_moves.push_back(m.first);
        }
    }
    if(!top_moves.size()) {
        for(auto m:score_moves) {
            if(m.second>0) {
                top_moves.push_back(m.first);
            }
        }
    }
    for(auto m:top_moves) {
        auto game_copy_1 = GameState(game_copy);
        game_copy_1.apply_move(m,game_copy_1);
        auto value = minmax(game_copy_1,-10e5,10e5,depth-1,!is_max_state);
        // std::cout<<value<<std::endl;
        if((is_max_state && value>best_value) || (!is_max_state && value<best_value)) {
            best_value = value;
            best_move = m;
        }

    }
    if(best_move.m_point==-1 && best_move.m_point_==-1) {
        return top_moves[0];
    }
    return best_move;

}

