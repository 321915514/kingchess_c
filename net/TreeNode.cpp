//
// Created by wangxu on 2024/3/16.
//

#include "TreeNode.h"
#include <queue>
#include <vector>
void TreeNode::expand(std::vector<std::pair<std::string,double>> action_priors) {
    for(auto item:action_priors){
        if(children.find(item.first) == children.end()){
            TreeNode* node = new TreeNode(this,item.second);
            children.insert(std::make_pair(item.first,node));
        }
    }
}


double TreeNode::get_value(const float c_puct) const{
    double parent_v = sqrt(this->parent->visits) / (1+ visits);
    return  (c_puct*P*parent_v) + this->Q;
}
struct CompareObjects {
    bool operator()(const TreeNode& a, const TreeNode& b) const {
        return a.get_value(5) < b.get_value(5);
    }
};

void TreeNode::select(TreeNode*& node){
    std::priority_queue<std::pair<std::string,TreeNode*>,std::vector<TreeNode*>,CompareObjects> q;
    for(auto item:children){
        q.push(item.second);
    }
    node = q.top();
}

void TreeNode::update(const coord_t& leaf_value){
    visits++;
    this->Q += 1.0*(leaf_value- this->Q) / this->visits;
}

void TreeNode::update_recursive(const coord_t& leaf_value){
    if(parent != nullptr){
        this->parent->update_recursive(-leaf_value);
    }
    update(leaf_value);
}


