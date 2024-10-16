//
// Created by 32191 on 2024/8/23.
//




#ifndef EXPERT_H
#define EXPERT_H


#include "board.h"
#include "function.h"
#include "gamestate.h"



class Expert_agent {
public:
    static std::unordered_map<Move,double,hashfunction,equalunction> select_move(GameState &game);
};



#endif //EXPERT_H
