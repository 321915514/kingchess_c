//
// Created by wangxu on 2024/3/16.
//

#include "mcts_alphazero.h"
#include "TreeNode.h"

void MCTS::playout(GameState& gameState){
    TreeNode* node = root;
    while (1){
        if(node->is_leaf()){
            break;
        }
        TreeNode* node;
        node->select(node)
    }
}

void MCTS::get_move_probs(GameState& gameState,const float temp){

}

void MCTS::update_with_move(Move& move){

}