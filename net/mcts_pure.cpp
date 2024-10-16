//
// Created by philxling on 2024/9/7.
//

#include "mcts_pure.h"
#include <float.h>
// TreeNode
TreeNode::TreeNode()
        : parent(nullptr),
          is_leaf(true),
//          virtual_loss(0),
          n_visited(0),
          p_sa(0),
          q_sa(0) {}

TreeNode::TreeNode(TreeNode *parent, double p_sa)
        : parent(parent),
          children{},
          is_leaf(true),
//          virtual_loss(0),
          n_visited(0),
          q_sa(0),
          p_sa(p_sa) {}

TreeNode::TreeNode(
        const TreeNode &node) {  // because automic<>, define copy function
    // struct
    this->parent = node.parent;
    this->children = node.children;
    this->is_leaf = node.is_leaf;

    this->n_visited.store(node.n_visited.load());
    this->p_sa = node.p_sa;
    this->q_sa = node.q_sa;

//    this->virtual_loss.store(node.virtual_loss.load());
}

TreeNode &TreeNode::operator=(const TreeNode &node) {
    if (this == &node) {
        return *this;
    }
    // struct
    this->parent = node.parent;
    this->children = node.children;
    this->is_leaf = node.is_leaf;

    this->n_visited.store(node.n_visited.load());
    this->p_sa = node.p_sa;
    this->q_sa = node.q_sa;
//    this->virtual_loss.store(node.virtual_loss.load());

    return *this;
}


MCTS_Pure::MCTS_Pure(unsigned int thread_num, double c_puct,

                     unsigned int num_mcts_sims)
        : thread_pool(new ThreadPool(thread_num)),
          c_puct(c_puct),
          num_mcts_sims(num_mcts_sims),
          c_virtual_loss(c_virtual_loss),
        //action_size(action_size),
          root(new TreeNode(nullptr, 1.), MCTS_Pure::tree_deleter),
          node_num(0) {}

void MCTS_Pure::update_with_move(int last_action) {
    auto old_root = this->root.get();

    // reuse the child tree
    if (last_action >= 0 && old_root->children[last_action] != nullptr) {
        // unlink
        TreeNode *new_node = old_root->children[last_action];
        old_root->children[last_action] = nullptr;
        new_node->parent = nullptr;

        this->root.reset(new_node);
    } else {
        this->root.reset(new TreeNode(nullptr, 1.));
    }
}

void MCTS_Pure::tree_deleter(TreeNode *t) {
    if (t == nullptr) {
        return;
    }
    // remove children
    for (unsigned int i = 0; i < t->children.size(); i++) {
        if (t->children[i]) {
            tree_deleter(t->children[i]);
        }
    }
    // remove self
    delete t;
}

int random_play(std::shared_ptr<GameState> game) {
    int winner = 0;
    while (true) {
        game->is_gameover(winner);
        if (winner == -1) {
            return WHITE;
        } else if (winner == 1) {
            return BLACK;
        }
        Move move;
        Bot::select_move(*game, move);
        game->apply_move(move, *game);
    }
}
unsigned int TreeNode::select(std::shared_ptr<GameState> game, double c_puct) {
    double best_value = -DBL_MAX;
    unsigned int best_move = 0;

    for (auto &child: children) {
        unsigned int sum_n_visited = this->n_visited.load() + 1;
        double cur_value = child.second->get_value(c_puct, sum_n_visited);
        if (cur_value > best_value) {
            best_value = cur_value;
            best_move = child.first;
        }
    }

    return best_move;
}

void TreeNode::expand(const std::unordered_map<int, float> &action_priors) {
    {
        std::lock_guard<std::mutex> lock(this->lock);
        if (this->is_leaf) {

            for (auto pair: action_priors) {
                this->children[pair.first] = new TreeNode(this, pair.second);
            }

            this->is_leaf = false;
            return;


        }
    }
}

void TreeNode::backup(double value) {
    if (this->parent != nullptr) {
        this->parent->backup(-value);
    }
    {
        std::lock_guard<std::mutex> lock(this->lock);
        this->q_sa = (n_visited * this->q_sa + value) / (n_visited + 1);
    }
    n_visited++;
}


double TreeNode::get_value(double c_puct, unsigned int sum_n_visited) const {
    auto n_visited = this->n_visited.load();
    double u = (c_puct * this->p_sa * sqrt(sum_n_visited) / (1 + n_visited));
    if (n_visited <= 0) {
        return u;
    } else {
        return u + (this->q_sa * n_visited) / n_visited;
    }
}

void MCTS_Pure::simulate(std::shared_ptr<GameState> game) {
    // execute one simulation
    auto node = this->root.get();

    std::vector<Move> moves;

    while (true) {

        moves.clear();
        game->legal_moves(moves);

        if (node->get_is_leaf()) {
            break;
        } else if (moves.size() == 0) {
            //std::cout<<"move size ==0";
            game->move.m_point = -100;
            game->move.m_point_ = -100;
            break;
        }

        auto action = node->select(game, this->c_puct);

        game->apply_move(game->action_2_move(action), *game);

        node = node->children.at(action);
    }
        // get game status
        int winner = 0;
        game->is_gameover(winner);
        float value;

        // not end
        if (winner == 0) {
            // predict action_probs and value by average
            std::vector<Move> legal_move;
            game->legal_moves(legal_move);

            std::unordered_map<int, float> probs;
            for (auto move: legal_move) {
                probs[game->move_2_action(move)] = 1.0f / static_cast<float>(legal_move.size());;
            }
            auto game_copy = std::make_shared<GameState>(*game);
            value = random_play(game_copy);

            node->expand(probs);
        } else {

            value = (winner == game->player ? 1 : -1);
        }
        node->backup(-value);
        return;

}


std::vector<float> MCTS_Pure::get_action_probs(GameState *game, const bool &show, double temp) {
    std::vector<std::future<void>> futures;

    for (unsigned int i = 0; i < this->num_mcts_sims; i++) {

        auto game_copy = std::make_shared<GameState>(*game);

//        simulate(game_copy);

        auto future = this->thread_pool->commit(std::bind(&MCTS_Pure::simulate, this, game_copy));

        futures.emplace_back(std::move(future));
    }

    // wait simulate
    for (unsigned int i = 0; i < futures.size(); i++) {
        futures[i].wait();
    }
    std::vector<Move> s;
    game->legal_moves(s);
    std::vector<float> action_probs(1125, 0);
    const auto &children = this->root->children;

    if (temp - 1e-3 < FLT_EPSILON) { // 返回最大的位置概率的索引action
        unsigned int max_count = 0;
        unsigned int best_action = 0;

        for (auto child: children) {
            if (child.second->n_visited.load() > max_count) {
                max_count = child.second->n_visited.load();
                best_action = child.first;
            }
        }

        action_probs[best_action] = 1.;
        return action_probs;

    } else {

        double sum = 0;
        for (auto child: children) {
            if (show) {
                std::cout <<"move:"<<Move::move2str(game->action_2_move(child.first))<< " child visit:" << child.second->n_visited.load() << "\n";
            }
            if (child.second->n_visited.load() > 0) {
                //std::cout<<children[i]->n_visited.load()<<std::endl;
                action_probs[child.first] = pow(child.second->n_visited.load(), 1 / temp);
                sum += action_probs[child.first];
            }
        }
        std::for_each(action_probs.begin(), action_probs.end(),
                      [sum](float &x) { x /= sum; });

        return action_probs;
    }
}