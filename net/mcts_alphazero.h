//
// Created by wangxu on 2024/3/16.
//

#ifndef KINGCHESS_MCTS_ALPHAZERO_H
#define KINGCHESS_MCTS_ALPHAZERO_H
#include "TreeNode.h"
#include "../fundamental/gamestate.h"
#include "../fundamental/move.h"
//#include "../model/model.h"
class MCTS {
public:
//    TRTModelLoader modelLoader;

    TreeNode* root;
    double c_puct;
    int n_playout;
    MCTS(TreeNode* treeNode,const double c_puct,int n_playout,std::string& path):root(treeNode),c_puct(c_puct),n_playout(n_playout){}

    void playout(GameState& gameState);

    void get_move_probs(GameState& gameState,const float temp=1e-3);

    void update_with_move(Move& move);
};


#endif //KINGCHESS_MCTS_ALPHAZERO_H
