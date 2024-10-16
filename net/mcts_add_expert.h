#pragma once

#include <unordered_map>
#include <string>
#include <vector>
#include <thread>
#include <atomic>
#include <filesystem>

#include <queue>

#include <functional>
#include <condition_variable>
#include <future>

#include "../fundamental/gamestate.h"

#include "../fundamental/utils.h"
// #include "../net/deep_model.h"
//#include <gomoku.h>
#include "thread_pool.h"

//#include <libtorch.h>




class TreeNode {
 public:
  // friend class can access private variables
  friend class MCTS;

  TreeNode();
  TreeNode(const TreeNode &node);
  TreeNode(TreeNode *parent, double p_sa);

  TreeNode &operator=(const TreeNode &p);

  unsigned int select(std::shared_ptr<GameState> game,double c_puct, double c_virtual_loss);
  void expand(const std::unordered_map<int, float> &action_priors,unsigned int& node_num);
  void backup(double leaf_value);

  double get_value(double c_puct, double c_virtual_loss,
                   unsigned int sum_n_visited) const;
  inline bool get_is_leaf() const { return this->is_leaf; }


//  bool fileExists(const std::string& filename){
//    return  fs::exists(filename);
//  }


 private:
  // store tree
  TreeNode *parent;
  std::unordered_map<int, TreeNode *> children;
  bool is_leaf;
  std::mutex lock;

  std::atomic<unsigned int> n_visited=0;
  double p_sa=0;
  double q_sa=0;
  std::atomic<int> virtual_loss=0;
};

class MCTS {
 public:

  // MCTS(deep_model *neural_network, unsigned int thread_num, double c_puct,
  //      unsigned int num_mcts_sims, double c_virtual_loss); // deep_model *neural_network

  MCTS(unsigned int thread_num, double c_puct, unsigned int num_mcts_sims, double c_virtual_loss);





 double generateRandom() {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<double> dis(-1.0, 1.0);
  return dis(gen);
 }


  std::vector<float> get_action_probs(GameState *game, const bool& show, double temp = 1e-3);
  void update_with_move(int last_move);

 private:
  void simulate(std::shared_ptr<GameState> game);
  static void tree_deleter(TreeNode *t);

  // variables
  std::unique_ptr<TreeNode, decltype(MCTS::tree_deleter) *> root;
  std::unique_ptr<ThreadPool> thread_pool;
  // deep_model *neural_network;
  //unsigned int action_size;
  unsigned int num_mcts_sims;
  double c_puct;
  double c_virtual_loss;
  std::mutex lock;
  unsigned int node_num;
};
