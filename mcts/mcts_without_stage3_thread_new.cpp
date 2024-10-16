//
// Created by 小魏 on 2023/10/29.
//

#include "mcts_without_stage3_thread_new.h"
#include "../fundamental/bot_vs_bot_random_simple_mcts_without_stage3.h"


Node* Mcts_bot::select_child(Node* node) {
    int N = 0;
    for (Node *n:node->children) {
        N += n->visit_count;
    }
    N = std::max(N,1);
    double best_score = -1000.0000;
    Node* best_child = nullptr;

    for (Node* n : node->children) {
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

int16_t Mcts_bot::simulate(GameState& s_game) {
    // 模拟走子
    //GameState s_game(*s);
    //GameState s1_game(s);
    //GameState s2_game(s_game);
    //std::cout << node->game.board.num_cols << std::endl;
    //std::cout << this << std::endl;
    //std::unordered_map<Player, Bot> bots = { {Player::black,Bot()},{Player::white,Bot()} };
    //Player winner = Player::none;
    //Move move = Move(Point(0,0));

//        Bot bot1;
//        Bot bot2;
    //std::cout << game.is_over() << std::endl;
    //GameState s_game(game);
//        std::cout << s_game.is_over() << std::endl;

    //const GameState s();
//    std::lock_guard<std::mutex> lockGuard(mutex);
//    std::cout << "simulate" << std::endl;
    return bot_vs_bot(s_game);

}

void Mcts_bot::backup(int16_t winner,Node* temp) {
//    std::lock_guard<std::mutex> lockGuard(mutex);
    Node* node = temp;
    while (node != nullptr) {
        node->win_count[winner]++;
        //std::cout << node->visit_count << std::endl;
        node->visit_count++;
        node = node->parent;
    }
//        delete node;

}


