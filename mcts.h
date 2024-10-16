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




#include "kingchess_c/fundamental/gamestate.h"

#include "kingchess_c/fundamental/utils.h"
//#include <gomoku.h>
#include "thread_pool.h"
#include "deep_model.h"
//#include <libtorch.h>

namespace fs = std::filesystem;


class TaskQueue {
public:
    friend class MCTS;
    TaskQueue():stop(false) {
        for (int i = 0; i < 4; ++i) {
            workers.emplace_back([this] {
                while (true) {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(this->queue_mutex);
                        this->condition.wait(lock, [this] { return this->stop || !this->tasks.empty(); });
                        if (this->stop && this->tasks.empty())
                            return;
                        task = std::move(this->tasks.front());
                        this->tasks.pop();
                    }
                    task();
                }
            });
        }
    }
    TaskQueue& operator=(const TaskQueue& other_q) {
        if (this == &other_q) {
            return *this;
        }
        TaskQueue();
        return *this;
    }





    ~TaskQueue() {
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            stop = true;
        }
        condition.notify_all();
        for (std::thread &worker : workers) {
            worker.join();
        }
    }
    template<class F, class... Args>
    auto enqueue(F&& f, Args&&... args)
        -> std::future<typename std::result_of<F(Args...)>::type> {
        using return_type = typename std::result_of<F(Args...)>::type;

        auto task = std::make_shared<std::packaged_task<return_type()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );

        std::future<return_type> res = task->get_future();
        {
            std::unique_lock<std::mutex> lock(queue_mutex);

            // don't allow enqueueing after stopping the pool
            if(stop)
                throw std::runtime_error("enqueue on stopped TaskQueue");

            tasks.emplace([task](){ (*task)(); });
        }
        condition.notify_one();
        return res;
    }

private:
    // need to keep track of threads so we can join them
    std::vector<std::thread> workers;
    // the task queue
    std::queue<std::function<void()>> tasks;

    // synchronization
    std::mutex queue_mutex;
    std::condition_variable condition;
    bool stop;

};



class TreeNode {
 public:
  // friend class can access private variables
  friend class MCTS;

  TreeNode();
  TreeNode(const TreeNode &node);
  TreeNode(TreeNode *parent, double p_sa);

  TreeNode &operator=(const TreeNode &p);

  unsigned int select(std::shared_ptr<GameState> game,double c_puct, double c_virtual_loss);
  void expand(const std::vector<float> &action_priors,const bool& full_search,unsigned int& node_num);
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
  std::vector<TreeNode *> children;
  bool is_leaf;
  std::mutex lock;

  std::atomic<unsigned int> n_visited=0;
  double p_sa=0;
  double q_sa=0;
  std::atomic<int> virtual_loss=0;
};

class MCTS {
 public:
  //TaskQueue q(4);
  friend class TaskQueue;
  MCTS(deep_model *neural_network, unsigned int thread_num, double c_puct,
       unsigned int num_mcts_sims, double c_virtual_loss);

    MCTS(unsigned int thread_num, double c_puct, unsigned int num_mcts_sims, double c_virtual_loss);


  bool fileExists(const std::string& filename){
    return fs::exists(filename);
  }


  std::vector<float> get_action_probs(GameState *game, const bool& show,const bool& full_search, double temp = 1e-3);
  void update_with_move(int last_move);

 private:
  void simulate(std::shared_ptr<GameState> game, const bool& full_search,unsigned int& node_num);
  static void tree_deleter(TreeNode *t);

  // variables
  std::unique_ptr<TreeNode, decltype(MCTS::tree_deleter) *> root;
  std::unique_ptr<ThreadPool> thread_pool;
  deep_model *neural_network;
 // TaskQueue q;
  //unsigned int action_size;
  unsigned int num_mcts_sims;
  double c_puct;
  double c_virtual_loss;
  std::mutex lock;
  unsigned int node_num;
};
