//
// Created by wangxu on 2023/8/7.
//

#ifndef TIC_MCTS_UTILS_H
#define TIC_MCTS_UTILS_H
#include <string>
#include <memory>
#include "coordinate.h"
#include "board.h"
#include "gamestate.h"

coord_t point_from_coords(const std::string& coords);
void print_move(int16_t player, const Move& move);
void print_move(const Move& move);
void print_board(const Board& board);
coord_t get_random_index(size_t max);
void encoder_data(std::shared_ptr<GameState> game, float (&grid)[81*5]);
void encoder_data_45(std::shared_ptr<GameState> game, float (&grid)[45*5]);
void encoder_board(std::shared_ptr<GameState> game, float (&grid)[81]);
#endif //TIC_MCTS_UTILS_H
