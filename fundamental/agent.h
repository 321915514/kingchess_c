//
// Created by wangxu on 2023/8/7.
//

#ifndef TIC_MCTS_AGENT_H
#define TIC_MCTS_AGENT_H

#include "board.h"
#include "coordinate.h"
#include "gamestate.h"
#include <vector>
#include <random>
#include <ctime>
#include "utils.h"
//class Bot {
//public:
//    static void select_move(GameState& game, Move &move) {
////        std::vector<coord_t> cans;
//
//        if (game.status == stage1) {
//            std::vector<coord_t> cans;
//            for (coord_t coord = 0; coord < Coordinate::m_square; ++coord) {
//                if (game.is_valid_move_stage1(Move::PlayStage1(coord))) {
//                    cans.push_back(coord);
//                }
//            }
//            move.m_point = cans[get_random_index(cans.size())];
//        } else if (game.status == stage2) {
//            std::unordered_map<coord_t, std::vector<std::vector<coord_t>>> v_moves;
//            coord_t move_first;
//            game.random_legal_moves_stage2(move_first,v_moves);
//            move.m_points.push_back(move_first);
//
////            coord_t square_front = 0;
////            if (game.is_square(move_frist, square_front)) {
////                move.m_squares_front = square_front;
////            }
//
////
//            auto item = v_moves.find(move_first)->second;
//
////            for(auto v_coord:item){
////                for(auto coord:v_coord){
////                    std::cout<<coord<<"\t";
////                }
////                std::cout<<std::endl;
////            }
//
//            if (!item.empty()) {
//                // print target move
//
////                auto max_index = item[0].size();
////                for(auto item1:item){
////                    max_index = std::max(max_index,item1.size());
////                }
////                int16_t item_index = 0;
////                for(int16_t i = 0; i<item.size();++i){
////                    if(max_index==item[i].size()){
////                        item_index = i;
////                    }
////                }
//
//
//                auto target_move = item[get_random_index(item.size())];
////                auto target_move = item[item_index];
//
//
//                if (target_move.size() > 1) {
//                    target_move.erase(target_move.begin());
//                    for (auto i:target_move) {
//                        move.m_points.push_back(i);
//                    }
//                } else {
//                    move.m_points.push_back(target_move[0]);
//                }
//            }
//        } else if(game.status==stage3){
//            //
//            std::vector<coord_t> cans;
//            std::unordered_map<coord_t, std::vector<std::vector<coord_t>>> v_moves;
//            coord_t move_first;
//            game.get_random_move_frist(move_first,v_moves);
//            move.m_points.push_back(move_first);
//
//            for (coord_t coord = 0; coord < Coordinate::m_square; ++coord) {
//                if (game.is_valid_move_stage3(coord)) {
//                    cans.push_back(coord);
//                }
//            }
//            auto& round = v_moves.find(move_first)->second;
//            for(auto coord:cans){
//                std::vector<coord_t> tmp;
//                tmp.push_back(coord);
//                round.push_back(tmp);
//            }
//            move.m_points.push_back(round[get_random_index(round.size())][0]);
//        }
//    }
//};


class Bot{
public:
    static void select_move(GameState& game, Move &move);
};


#endif
