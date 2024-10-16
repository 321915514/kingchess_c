//
// Created by philxling on 2024/9/7.
//
#include "board.h"
#include "gamestate.h"

int main(){




    Board board(5,9);

    auto result = board.check_move1(10,18);


    std::cout<<result<<std::endl;



}