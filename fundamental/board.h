//
// Created by wangxu on 2023/8/7.
//



#ifndef TIC_MCTS_BOARD_H
#define TIC_MCTS_BOARD_H
#include <cstring>
#include "coordinate.h"
#include "move.h"
#include <unordered_map>
#include <iostream>
#include <functional>
#include <cstdint>
#include <set>
//#include "function.h"
#include "neighbor.h"
#include "zobrist.h"
#define MOVE_BUFFER_SIZE 1000

class Board{

public:
    int16_t grid[SQUARE];

public:
    Board(){}



    Board(int16_t row,int16_t col) {
        memset(grid,0,sizeof(grid));
        grid[12] = -1;
        grid[13] = -1;
        grid[14] = -1;

        grid[20] = 1;

        grid[21] = -1;
        grid[23] = -1;

        grid[24] = 1;

        grid[30] = -1;
        grid[31] = -1;
        grid[32] = -1;

        Coordinate::SetBoardSize(row,col);

        Neighbor4::Init();
        DiagNeighbor4::Init();
        Neighbor8::Init();
        Neighbor12::Init();
        FarNeighbor4::Init();
        Neighbor20::Init();
        Neighbor36::Init();
        Neighbor44::Init();
        Neighbor68::Init();

        AllNeighbor4::Init();
        AllDiagNeighbor4::Init();
        AllNeighbor8::Init();
        AllNeighbor12::Init();
        AllNeighbor20::Init();
        AllFarNeighbor4::Init();

        zobrist.Init();
        nakade.Init();
    }

    Board(const Board& other_board){
        memcpy(grid,other_board.grid,sizeof(other_board));
    }
    static const std::set<std::pair<coord_t, coord_t>> invalid_moves;
    static const std::set<std::pair<coord_t, coord_t>> blocked_paths;
//    bool operation_square_simple(const int16_t& player,const int16_t& squares);


    Board& operator=(const Board& other_board) {
        if (this == &other_board) {
            return *this;
        }
        memcpy(grid,other_board.grid,sizeof(other_board.grid));
        return *this;
    }


    void stone_down(const coord_t& coord,int16_t player) ;

    coord_t stone_go(const coord_t& coord,const coord_t& coord_,int16_t player) ;

    bool is_jump(const coord_t& coord,const coord_t& coord_) const;

    coord_t count_chess(const int16_t& player) const;

//    bool operation_square(const int16_t& player,const int16_t& squares) ;

    bool check_move1(const coord_t& point,const coord_t& point_) const;

    bool legal_move(const coord_t& point,const coord_t& point_) const;


};


#endif
