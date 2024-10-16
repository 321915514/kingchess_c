//
// Created by dev on 4/9/24.
//
#include <chrono>
#include "kingchess_c/fundamental/coordinate.h"
#include "gamestate.h"
#include "agent.h"
#include "utils.h"
int main(){
    GameState game;
    GameState::new_game(5,9,game);
    float grid[81*5] = {0};
    auto game_copy = std::make_shared<GameState>(game);
    encoder_data(game_copy,grid);

    for(auto i:grid){
        std::cout<<i<<"\t";
    }


}
