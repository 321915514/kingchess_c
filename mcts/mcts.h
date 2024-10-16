//
// Created by wangxu on 2023/8/7.
//

#ifndef TIC_MCTS_MCTS_H
#define TIC_MCTS_MCTS_H

#include "../fundamental/coordinate.h"
#include "../fundamental/board.h"
#include "../fundamental/agent.h"
//#include "../fundamental/bot_vs_bot_random_simple_mcts.h"
#include <unordered_map>
#include "../fundamental/utils.h"
#include <vector>
#include <random>
#include <cmath>
#include <memory>
#include <iostream>
#include <typeinfo>
#include <algorithm>
#include <set>
#include "node.h"



class Mcts_bot {
public:
    int rounds;
    Mcts_bot(int rounds):rounds(rounds){}

    Node* select_child(Node* node) const{
        int N = 0;
        for (Node *n:node->children) {
            N += n->visit_count;
        }
        N = std::max(N,1);
        double best_score = -1000.0000;
        Node *best_child = nullptr;

        for (Node *n : node->children) {
            // 接着写
            double win = static_cast<double>(n->win_count[n->game.player]) / static_cast<double>(n->visit_count <= 0 ? 1e-10 : n->visit_count);

            //std::cout << n.visit_count << std::endl;

            double explo = sqrt(2) * sqrt(log(N) / (n->visit_count <= 0 ? 1e-10 : n->visit_count));

            double score = win + explo;

            if (score > best_score) {
                best_score = score;
                best_child = n;
            }

        }
        return best_child;


    }

    int16_t simulate(GameState game) {
        // 模拟走子
//        GameState game;
//        GameState::new_game(5,9,game);

//        print_board(game.board);

//        std::cout<< "simulate" << std::endl;

        while (true){
//            print_board(game.board);
//            std::cout<<game.eat_chess()<<std::endl;

//            if(game.eat_chess() == 1){
//                print_board(game.board);
//            }

            if(game.eat_chess()>=11){
//            std::cout<<"black win !!"<<std::endl;
//                std::cout<< "simulate  end" << std::endl;
                return BLACK;
            }
            Move move;
            if(game.player == BLACK){

                Bot::select_move(game,move);

//                std::cout<< "black select"<<std::endl;

                if(move.m_point == -100 && move.m_point_ == -100){
//                std::cout<<"white win !!"<<std::endl;

//                    std::cout<< "simulate  end" << std::endl;
                    return WHITE;
                }
//            print_move(game.player,move);
            } else{
                Bot::select_move(game,move);
//                std::cout<< "white select"<<std::endl;
//            print_move(game.player,move);
            }
            game.apply_move(move,game);

        }
        if(game.move.m_point == -1 && game.move.m_point_ == -1){
//                std::cout<<"white win !!"<<std::endl;
            std::cout<< "simulate  end" << std::endl;
            return WHITE;
        }else if(game.eat_chess()>=11){
//            std::cout<<"black win !!"<<std::endl;

            std::cout<< "simulate  end" << std::endl;
            return BLACK;
        }


    }

    void backup(int16_t winner,Node *temp) {
        Node *node = temp;
        while (node != nullptr) {
            node->win_count[winner]++;
            //std::cout << node->visit_count << std::endl;
            node->visit_count++;

            node=node->parent;
        }
//        delete node;

    }

    void select_move(GameState& g,Move& move) {
//        std::cout << static_cast<int>(g.player) << std::endl;
        Node *root = new Node(g);
//        std::cout << static_cast<int>(root.game.player) << std::endl;
        for (int i = 0; i < rounds; i++) {
            Node *node = root;

            while ((!(node->is_Terminal()) && !(node->unvisited_moves.size() > 0))) {
                node = select_child(node);
            }//            for(Node* entry:root->children){

            //                std::cout<<entry->move.point.row<<"--"<<entry->move.point.col<<"\t";
//            }
//            std::cout<<std::endl;
//            Node *temp = new Node(*root);
            if (node->can_add_child()) {
                node = node->add_child(node->game);
            }//            std::cout<<node<<std::endl;
            int16_t winner = simulate(node->game);
            if (winner == BLACK || winner == WHITE) {
                backup(winner, node);
            }
        }

//        std::cout<< "mcts end"<<std::endl;

        std::vector<std::tuple<double, Move, int>> scored_moves;

        int N = 0;
//        std::cout << root.children.size() << std::endl;

         for (Node *child : root->children) {
            N += child->visit_count;
        }

        N = std::max(1, N);

//        std::cout << N << std::endl;


        for (Node *child : root->children) {
            double win = static_cast<double>(child->win_count[root->game.player]) / std::max(1e-10, static_cast<double>(child->visit_count));

            double explo = std::sqrt(2) * std::sqrt(std::log(N) / std::max(1e-10, static_cast<double>(child->visit_count)));

            double score = win + explo;

            scored_moves.emplace_back(score, child->move, child->visit_count);
        }

        std::sort(scored_moves.begin(), scored_moves.end(),
                  [](const auto& a, const auto& b) { return std::get<0>(a) > std::get<0>(b); });

        for (const auto& scored_move : scored_moves) {
//            double score = std::get<0>(scored_move);
//            Move move = std::get<1>(scored_move);
//            int visit = std::get<2>(scored_move);
//            print_move(std::get<1>(scored_move));
            std::string move_str;
            if(Move::is_down(std::get<1>(scored_move))){
                move_str = "point(row="+std::to_string(std::get<1>(Coordinate::GetXY(std::get<1>(scored_move).m_point)))+",col="+std::to_string(std::get<0>(Coordinate::GetXY(std::get<1>(scored_move).m_point)))+")";
//                std::cout << move_str << std::endl;
            } else if(Move::is_go(std::get<1>(scored_move))) {
                move_str = "point(row=" + std::to_string(std::get<1>(Coordinate::GetXY(std::get<1>(scored_move).m_point))) + ",col=" +
                            std::to_string(std::get<0>(Coordinate::GetXY(std::get<1>(scored_move).m_point))) + ")"
                            + "--------point(row=" + std::to_string(std::get<1>(Coordinate::GetXY(std::get<1>(scored_move).m_point_))) +
                            ",col=" + std::to_string(std::get<0>(Coordinate::GetXY(std::get<1>(scored_move).m_point_))) + ")";
            }
            std::cout<<move_str<<"--------------"<<std::get<0>(scored_move)<<"("<<std::get<2>(scored_move)<<")"<<std::endl;
        }
//        print_move(std::get<1>(scored_moves[0]));
//        std::cout << "Select move " << std::get<1>(scored_moves[0]).point.row << "," << std::get<1>(scored_moves[0]).point.col << " with win pct " << std::get<0>(scored_moves[0]) << "\n";
         if(Move::is_down(std::get<1>(scored_moves[0]))){
             move.m_point = std::get<1>(scored_moves[0]).m_point;
         }else if(Move::is_go(std::get<1>(scored_moves[0]))){
             move.m_point = std::get<1>(scored_moves[0]).m_point;
             move.m_point_ = std::get<1>(scored_moves[0]).m_point_;
         }
    }

};

#endif //TIC_MCTS_MCTS_H
