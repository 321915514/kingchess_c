//
// Created by wangxu on 2024/4/11.
//
#include "coordinate.h"
#include "board.h"
#include "utils.h"
#include "gamestate.h"
#include <iomanip>
#include <cstring>


int main() {

    GameState game;
    GameState::new_game(5,9,game);

    float grid[81*5] = {0};
    float grid_board[81] = {0};
//    std::vector<std::vector<int32_t>> temp;
//    temp.push_back(std::vector<int32_t>(grid,grid+81));
//
//    for(auto item:temp){
//        for(auto item1:item){
//            std::cout<<item1<<'\t';
//        }
//        std::cout<<std::endl;
//    }

    auto game_copy = std::make_shared<GameState>(game);

    encoder_data(game_copy,grid);
    encoder_board(game_copy,grid_board);
    //float* twoDArray[5];
    //for (int i = 0; i < 5; ++i) {
    //    twoDArray[i] = &grid[i * 81];
    //}

    // 现在可以使用twoDArray[row][col]来访问元素了
    //for (int row = 0; row < 5; ++row) {
    //    for (int col = 0; col < 81; ++col) {
    //        std::cout << twoDArray[row][col] << " ";
    //    }
    //    std::cout << std::endl;
    //}












    //for (int row = 0; row < 2; ++row) {
    //    for (int col = 0; col < 81; ++col) {
    //        twoDArray[row][col]  = Coordinate::m_symmetry[col][1];
    //    }
    //    std::cout << std::endl;
    //}

    //std::cout<<std::endl;

    //for (int row = 0; row < 5; ++row) {
    //    for (int col = 0; col < 81; ++col) {
    //        std::cout << twoDArray[row][col] << " ";
    //    }
    //    std::cout << std::endl;
    //}

    //for (coord_t coord = 0; coord < 81; ++coord) {
    //    std::cout<< grid_board[coord]<<'\t';
    //}

    // board



    float temp[81];

    for (coord_t coord = 0; coord < 81; ++coord) {
        temp[coord]  = grid_board[Coordinate::m_symmetry[coord][1]]; // 返回
    }
    memcpy(grid_board,temp,sizeof(temp));
    std::cout << std::endl;

    for (coord_t coord = 0; coord < 81; ++coord) {
        std::cout<< grid_board[coord]<<'\t';
    }


    std::cout << std::endl;





//
//    for(auto item:temp){
//        for(auto item1:item){
//            std::cout<<item1<<'\t';
//        }
//        std::cout<<std::endl;
//    }

}
