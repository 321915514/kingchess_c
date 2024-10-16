//
// Created by wangxu on 2023/10/30.
//

#ifndef LONGCHESS_NODE_H
#define LONGCHESS_NODE_H

#include <mutex>
#include <memory>
#include <utility>
#include "../fundamental/gamestate.h"
#include "../fundamental/utils.h"
#include "rw_lock.h"
class Node{
public:
    std::unordered_map<int16_t, int> win_count;
    int visit_count = 1;
//    int move_length = 0;
    GameState game;
    Node* parent;
    Move move;
    RWLock rwLock;
    std::mutex mutex;
//    coord_t move_first;
    std::vector<Move> unvisited_moves;
//    std::unordered_map<coord_t, std::vector<std::vector<coord_t>>> unvisited_moves_stage2;
//    std::unordered_map<coord_t, std::vector<std::vector<coord_t>>> unvisited_moves_stage3;
//    std::vector<coord_t> moves_first;
    std::vector<Node*> children;

    Node(GameState game, Node* parent = nullptr, const Move &move=Move()):
            game(game),
            parent(parent),
            move(move),
            win_count{ {BLACK,0},{WHITE,0}},
            children()
    {
        game.legal_moves(unvisited_moves);
//        if(game.status==stage1){
//            unvisited_moves_stage1 = game.legal_moves_stage1();
//        } else if(game.status==stage2){
//            game.mcts_legal_moves_stage2(unvisited_moves_stage2);
//            for (auto it:unvisited_moves_stage2) {
//                moves_first.push_back(it.first);
//            }
//        }

    }



    ~Node(){
        for (Node* child : children) {
            delete child; // 释放子节点的内存
        }
    }


    //拷贝构造函数
    Node(const Node& other) :
            visit_count(other.visit_count),
            game(other.game), // 需要调用 GameState 的拷贝构造函数
            move(other.move),
            parent(other.parent),
            unvisited_moves(other.unvisited_moves),
            children(other.children) // 需要调用 std::vector<Node> 的拷贝构造函数
    {

        for (const auto& entry : other.win_count) {
            win_count.insert(entry); // 注意：这里会执行键值对的拷贝
        }


    }

    // 赋值运算符
    Node& operator=(const Node& other) {
        if (this == &other) {
            return *this;
        }

        win_count = other.win_count;
        visit_count = other.visit_count;
        game = other.game; // 需要调用 GameState 的赋值运算符
        parent = other.parent;
//        parent = other.parent;
        move = other.move;
        unvisited_moves = other.unvisited_moves;
        children = other.children; // 需要调用 std::vector<Node> 的赋值运算符


        return *this;
    };

    bool is_Terminal() {
        return game.is_gameover();
    }

    Node* add_child(GameState g) {
        // 随机加入未访问的节点
//        rwLock.WriteLock();
            int index = get_random_index(unvisited_moves.size());
            Move new_move = unvisited_moves[index];
            unvisited_moves.erase(unvisited_moves.begin() + index);

            for (Node* child : children) {
                if (child->move == new_move) {
//                    std::cout<<child<<std::endl;
                    return child;// 如果已存在，返回现有节点
                }
            }
//            GameState newgame;
            g.apply_move(new_move,g);
//            newnode.game = g;
//            newnode.parent = this;
//            newnode.move = new_move;
            Node *new_node = new Node(g, this, new_move);
            //Node(g, this,m)
//            auto new_node = std::make_shared<Node>(g,th);
//            rwLock.WriteLock();
            children.push_back(new_node);
//            rwLock.WriteUnlock();
            return new_node;

    }

    bool can_add_child(){
        /*
        检测当前是否还有合法动作未添加到树中
                :return:
        */
        {
            std::lock_guard<std::mutex> lockGuard(mutex);
            return !unvisited_moves.empty();
        }

    }

};


#endif //LONGCHESS_NODE_H
