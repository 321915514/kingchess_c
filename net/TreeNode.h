//
// Created by wangxu on 2024/3/16.
//

#ifndef KINGCHESS_TREENODE_H
#define KINGCHESS_TREENODE_H
#include <unordered_map>
#include <string>
#include "../fundamental/coordinate.h"
class TreeNode {
public:
    TreeNode* parent;
    std::unordered_map<std::string,TreeNode*> children;
    int visits = 0;
    double Q = 0;
    double U = 0;
    double P = 0;

    TreeNode(TreeNode* parent,double prior_p){
        this->parent = parent;
        this->P = prior_p;
        this->children = {};
    }

    void expand(std::vector<std::pair<std::string,double>> action_priors);

    void select(TreeNode*& node);

    void update(const coord_t& leaf_value);

    void update_recursive(const coord_t& leaf_value);

    void get_value(const float c_puct, double& result);

    bool is_leaf(){
        return this->children.empty();
    }

    bool is_root(){
        return this->parent == nullptr;
    }

    double get_value(const float c_puct) const;

};


#endif //KINGCHESS_TREENODE_H
