#pragma once

#include <random>
#include <unordered_map>
#include "coordinate.h"

#define VNULL NULL_COORD

typedef unsigned long long int zint64;

/**
 *  Class containing the table for Zobrist hash.
 */
struct Zobrist{
    zint64 hash[8][4][SQUARE];

    // axis=0: symmetrical index
    //         [0]->original [1]->90deg rotation, ... [4]->inverting, ...
    // axis=1: stone color index
    //         [0]->blank [1]->Ko [2]->white stone [3]->black stone
    // axis=2: vertex position

    Zobrist() {
        Init();
    }

    void Init() {
        std::mt19937_64 mt_64_(123);

        for (int i=0;i<8;++i){
            bool is_inverted = (i == 4);

            for (int j=0;j<4;++j) {
                for (int16_t k=0;k<Coordinate::m_square;++k) {

                    int16_t x = Coordinate::m_size - 1 - (k % Coordinate::m_size);
                    int16_t y = (k / Coordinate::m_size);

                    if (i == 0) {
                        hash[i][j][k] = mt_64_();
                    } else if (is_inverted) {
                        hash[i][j][k] = hash[0][j][x + y * Coordinate::m_size];
                    } else {
                        hash[i][j][k] = hash[i - 1][j][y + x * Coordinate::m_size];
                    }

                }
            }
        }

    }
};
extern Zobrist zobrist;

/**
 *  Class containing Nakade patterns and their vitals.
 *     key: Zobrist hash of blank vertexes.
 *     value: position of vital
 *
 *  Each positions are described by relative coordinate
 *  with the origin at the board center.
 *  Ex. v_rel = v - board_square / 2
 */
struct Nakade{

    std::unordered_map<zint64,int> vital;

    Nakade() {
        Init();
    }

    void Init() {
        vital.clear();
        int space_3[4][3] = {	{-1,0,1},			// 0 <- vital position
            {0,1,Coordinate::m_size},		// 0
            {0,1,2},			// 1
            {0,1,Coordinate::m_size+1}	};	// 1

        int space_4[4][4] = {	{-1,0,1,Coordinate::m_size},			// 0
            {0,1,-Coordinate::m_size+1,Coordinate::m_size+1},	// 1
            {0,1,2,Coordinate::m_size+1},			// 1
            {0,1,Coordinate::m_size,Coordinate::m_size+1}};		// 0

        int space_5[7][5] = {	{-1,0,1,Coordinate::m_size-1,Coordinate::m_size},			// 0
            {-1,0,1,-Coordinate::m_size,Coordinate::m_size},			// 0
            {0,1,2,Coordinate::m_size,Coordinate::m_size+1},			// 1
            {0,1,2,Coordinate::m_size+1,Coordinate::m_size+2},			// 1
            {0,1,-Coordinate::m_size+1,Coordinate::m_size,Coordinate::m_size+1},	// 1
            {0,1,2,-Coordinate::m_size+1,Coordinate::m_size+1},			// 1
            {0,1,Coordinate::m_size,Coordinate::m_size+1,Coordinate::m_size+2}};	// Coordinate::m_size+1

        int space_6[4][6] = {	{-Coordinate::m_size-1,-Coordinate::m_size,-1,0,1,Coordinate::m_size},			// 0
            {0,1,2,-Coordinate::m_size+1,Coordinate::m_size,Coordinate::m_size+1},			// 1
            {-Coordinate::m_size+1,0,1,2,Coordinate::m_size+1,Coordinate::m_size+2},		// 1
            {0,1,Coordinate::m_size,Coordinate::m_size+1,Coordinate::m_size+2,2*Coordinate::m_size+1}};	// Coordinate::m_size+1

        int vaital_3[4] = {0,0,1,1};
        int vaital_4[4] = {0,1,1,0};
        int vaital_5[7] = {0,0,1,1,1,1,2};
        int vaital_6[4] = {0,1,1,2};

        int sym_position[4][8] = {	{0,0,0,0,0,0,0,0},
            {1,-Coordinate::m_size,-1,Coordinate::m_size,-1,-Coordinate::m_size,1,Coordinate::m_size},
            {Coordinate::m_size+1,-Coordinate::m_size+1,-Coordinate::m_size-1,Coordinate::m_size-1,
                Coordinate::m_size-1,-Coordinate::m_size-1,-Coordinate::m_size+1,Coordinate::m_size+1},
            {VNULL,VNULL,VNULL,VNULL,VNULL,VNULL,VNULL,VNULL}	};

        int center = Coordinate::m_square / 2;
        zint64 space_hash = 0;

        for(int i=0;i<8;++i){
            for(int j=0;j<4;++j){
                space_hash = 0;
                for(int k=0;k<3;++k){
                    space_hash ^= zobrist.hash[i][0][center+space_3[j][k]];
                }
                vital.insert({space_hash, sym_position[vaital_3[j]][i]});
            }
            for(int j=0;j<4;++j){
                space_hash = 0;
                for(int k=0;k<4;++k){
                    space_hash ^= zobrist.hash[i][0][center+space_4[j][k]];
                }
                vital.insert({space_hash, sym_position[vaital_4[j]][i]});
            }
            for(int j=0;j<7;++j){
                space_hash = 0;
                for(int k=0;k<5;++k){
                    space_hash ^= zobrist.hash[i][0][center+space_5[j][k]];
                }
                vital.insert({space_hash, sym_position[vaital_5[j]][i]});
            }
            for(int j=0;j<4;++j){
                space_hash = 0;
                for(int k=0;k<6;++k){
                    space_hash ^= zobrist.hash[i][0][center+space_6[j][k]];
                }
                vital.insert({space_hash, sym_position[vaital_6[j]][i]});
            }
        }
    }
};

extern Nakade nakade;
/*
   class Board; // forward declaration of class in board.h

   int64 BoardHash(Board& b);
   int64 UpdateBoardHash(Board& b, int64 prev_hash);
   */
