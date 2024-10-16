#include <iostream>
#include <chrono>
#include "coordinate.h"
#include "neighbor.h"
#include "gamestate.h"
int main(){

    GameState game;
    GameState::new_game(5, 9, game);


    auto start = std::chrono::high_resolution_clock::now();
    for(int16_t i =0;i<10000;i++){
//        simple();
        std::vector<Move> moves;
        game.legal_moves(moves);
    }
//    run();
    auto end = std::chrono::high_resolution_clock::now();
    // 计算时间间隔
    std::chrono::duration<double> duration = end - start;
    // 输出时间间隔
    std::cout << "time " << duration.count() << "s" << std::endl;









}
