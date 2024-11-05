//
// Created by 32191 on 2024/9/30.
//

#ifndef ALPHA_BETA_H
#define ALPHA_BETA_H

#include "gamestate.h"
#include "expert.h"
#include <unordered_map>


class Alpha_beta{
public:
    double evaluation_state(GameState& game_state);
    double minmax(GameState& game,double alpha,double beta,int depth,bool is_max_depth);
    Move select_move(GameState& game,int depth);
    std::unordered_map<int,float> score_moves(GameState &game, int depth); 
};



#endif //ALPHA_BETA_H
