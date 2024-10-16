//
// Created by wangxu on 2024/3/16.
//

#ifndef KINGCHESS_MCTS_PLAYER_H
#define KINGCHESS_MCTS_PLAYER_H
#include "../fundamental/gamestate.h"

class MCTSPlayer {

public:
    bool is_self_play;

    MCTSPlayer(bool is_self_play):is_self_play(is_self_play){}

    void select_move(GameState& gameState,const float temp=1e-3,bool is_self_play=0);


};


#endif //KINGCHESS_MCTS_PLAYER_H
