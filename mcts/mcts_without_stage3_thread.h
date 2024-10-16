
#include "threadpool.h"
#include "../fundamental/coordinate.h"
#include "../fundamental/board.h"
#include "../fundamental/agent.h"
#include "../fundamental/bot_vs_bot_random_simple_mcts_without_stage3.h"
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
#include <future>
#include <mutex>

class Mcts_bot {
public:
    int rounds;
    ThreadPool threadPool;
    std::mutex mutex;
//    std::condition_variable cv;
//    int tasksCompleted = 0;

    Node* select_child(Node* node);

    void backup(int16_t winner,Node* temp);

    int16_t simulate(GameState& s_game);

    Mcts_bot(int rounds,int threadPool) : rounds(rounds),threadPool(threadPool){}

    void select_move(const GameState& g, Move& move) {
        auto root = new Node(g);
        std::vector<std::future<void>> futures;
        for (int i = 0; i < rounds; i++) {

            // 使用线程池执行MCTS模拟
            futures.emplace_back(threadPool.enqueue([this, root, i] {


                std::lock_guard<std::mutex> lockGuard(mutex);
//                std::cout << "thread_id:" << std::this_thread::get_id() << "i:" << i << std::endl;
//                   std::cout<<"i:"<<i<<std::endl;
                Node *node = root;
                // 您的MCTS模拟逻辑在这里，类似于原始代码
                if (node->game.status == stage1) {
//                    std::lock_guard<std::mutex> lockGuard(mutex);
                    while (node != nullptr && (!(node->is_Terminal()) && node->unvisited_moves_stage1.empty())) {
                        node = select_child(node);
                    }
                } else if (node->game.status == stage2) {
//                    std::lock_guard<std::mutex> lockGuard(mutex);
                    while (node != nullptr && (!(node->is_Terminal()) && node->unvisited_moves_stage2.empty())) {
                        node = select_child(node);
                    }
                }

//                std::cout << "thread_id_2:" << std::this_thread::get_id() << "i:" << i << std::endl;


                if (node != nullptr && node->can_add_child()) {
//                    std::lock_guard<std::mutex> lockGuard(mutex);
                    node = node->add_child(node->game);
                }
//                std::cout << "thread_id_3:" << std::this_thread::get_id() << "i:" << i << std::endl;

                if (node != nullptr) {
//                    std::lock_guard<std::mutex> lockGuard(mutex);
                    int16_t winner = simulate(node->game);
//                    std::cout << winner << std::endl;
                    if (winner == BLACK || winner == WHITE) {
//                        std::lock_guard<std::mutex> lockGuard(mutex);
                        backup(winner, node);
//                        std::cout<<"backup end"<<std::this_thread::get_id()<<std::endl;
                    }
                }

            }));

//            futures.emplace_back(std::async(std::launch::async,[this, root] {
//               Node* node = root;
//                // 您的MCTS模拟逻辑在这里，类似于原始代码
//                if(node->game.status==stage1){
//                    while (node != nullptr && (!(node->is_Terminal()) && node->unvisited_moves_stage1.empty())) {
//                        node = select_child(node);
//                    }
//                } else if(node->game.status==stage2){
//                    while (node != nullptr && (!(node->is_Terminal()) && node->unvisited_moves_stage2.empty()) ) {
//                        node = select_child(node);
//                    }
//                }
//                if (node != nullptr && node->can_add_child()) {
////                    std::lock_guard<std::mutex> lockGuard(mutex);
//                    node = node->add_child(node->game);
//                }
//                if(node != nullptr){
////                    std::lock_guard<std::mutex> lockGuard(mutex);
//                    int16_t winner = simulate(node->game);
////                    std::cout<<winner<<std::endl;
//                    if (winner == BLACK || winner == WHITE) {
//                        backup(winner, node);
//                    }
//                }
//            }));

//
//            threadPool.submit([this, root] {
//                std::shared_ptr<Node> node = root;
//                // 您的MCTS模拟逻辑在这里，类似于原始代码
//
//
//                if(node->game.status==stage1){
//                    while (node != nullptr && (!(node->is_Terminal()) && !(node->unvisited_moves_stage1.size()>0))) {
//                        node = select_child(node);
//                    }
//                } else if(node->game.status==stage2){
//                    while (node != nullptr && (!(node->is_Terminal()) && !(node->unvisited_moves_stage2.size()>0)) ) {
//                        node = select_child(node);
//                    }
//                }
//
//                if (node != nullptr && node->can_add_child()) {
////                    std::lock_guard<std::mutex> lockGuard(mutex);
//                    node = node->add_child(node->game);
//                }
//
//                if(node != nullptr){
////                    std::lock_guard<std::mutex> lockGuard(mutex);
//                    int16_t winner = simulate(node->game);
////                    std::cout<<winner<<std::endl;
//                    if (winner == BLACK || winner == WHITE) {
//                        backup(winner, node);
//                    }
//                }else{
//                    node = root;
//                }
//
//
//            });


        }

//        threadPool.waitAll();
        for (auto&& future : futures) {
            future.get();
        }
        // 选择最佳移动的逻辑在这里
//        std::cout<<"hello"<<std::endl;
        std::vector<std::tuple<double, Move, int>> scored_moves;

        int N = 0;
//        std::cout << root.children.size() << std::endl;

        for (Node* child : root->children) {
            N += child->visit_count;
        }

        N = std::max(1, N);


        for (Node* child : root->children) {
            double win = static_cast<double>(child->win_count[root->game.player]) / std::max(1e-10, static_cast<double>(child->visit_count));

            double explo = std::sqrt(2) * std::sqrt(std::log(N) / std::max(1e-10, static_cast<double>(child->visit_count)));

            double score = win + 0.9*explo;

            scored_moves.emplace_back(score, child->move, child->visit_count);
        }

        std::sort(scored_moves.begin(), scored_moves.end(),
                  [](const auto& a, const auto& b) { return std::get<0>(a) > std::get<0>(b); });

//        std::vector<std::tuple<double, Move, int>> firstTen(scored_moves.begin(), scored_moves.size()>=10? scored_moves.begin() + 10:scored_moves.end());

        for (const auto& scored_move : scored_moves) {
            double score = std::get<0>(scored_move);
            Move move = std::get<1>(scored_move);
            int visit = std::get<2>(scored_move);
//            print_move(move);
            if(move.m_point>=0){
                std::cout << "point(row="<<std::get<1>(Coordinate::GetXY(move.m_point))<< ",col=" << std::get<0>(Coordinate::GetXY(move.m_point))<<")"<< " - uct score " << score << " (" << visit << ")\n";
            } else{
                std::ostringstream oss;
                for(auto point:move.m_points){
                    oss << std::get<1>(Coordinate::GetXY(point)) <<
                        std::get<0>(Coordinate::GetXY(point)) << "--";

                }
                std::cout <<oss.str()<< " - uct score " << score << " (" << visit << ")\n";


            }
        }
        if(std::get<1>(scored_moves[0]).m_point>=0){
            std::cout << "point(row="<<std::get<1>(Coordinate::GetXY(std::get<1>(scored_moves[0]).m_point))<< ",col=" << std::get<0>(Coordinate::GetXY(std::get<1>(scored_moves[0]).m_point))<<") with win pct " << std::get<0>(scored_moves[0]) << "\n";
        } else{
            std::ostringstream oss;
            for(auto point:std::get<1>(scored_moves[0]).m_points){
                oss << std::get<1>(Coordinate::GetXY(point)) <<
                    std::get<0>(Coordinate::GetXY(point)) << "--";

            }
            std::cout <<oss.str()<< " - uct score " << std::get<0>(scored_moves[0]) << " (" << std::get<2>(scored_moves[0]) << ")\n";


        }
        if(Coordinate::OnBoard(std::get<1>(scored_moves[0]).m_point)){
            move.m_point = std::get<1>(scored_moves[0]).m_point;
        }else{
            for(auto i:std::get<1>(scored_moves[0]).m_points){
                move.m_points.push_back(i);
            }
        }

//        delete root;
    }
};

