#include <math.h>
#include <float.h>
#include <numeric>
#include <iostream>
#include "mcts.h"
#include <cassert>
#include "thirdparty/gflags/gflags.h"
#include "thirdparty/glog/logging.h"

//DEFINE_string(engine_path,"/home/dev/kingchess_remote/kingchess/net/model/current.engine","TensorRT engine file path");

    //deep_model model("/home/dev/kingchess_remote/kingchess/net/model/current.engine");
// TreeNode
TreeNode::TreeNode()
    : parent(nullptr),
      is_leaf(true),
      virtual_loss(0),
      n_visited(0),
      p_sa(0),
      q_sa(0) {}

TreeNode::TreeNode(TreeNode *parent, double p_sa)
      :parent(parent),
      children(),
      is_leaf(true),
      virtual_loss(0),
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

  this->virtual_loss.store(node.virtual_loss.load());
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
  this->virtual_loss.store(node.virtual_loss.load());

  return *this;
}

unsigned int TreeNode::select(std::shared_ptr<GameState> game,double c_puct, double c_virtual_loss) {
  //LOG(ERROR) << "select start";
  double best_value = -DBL_MAX;
  unsigned int best_move = 0;
  //TreeNode *best_node=nullptr;
    std::vector<Move> moves;
    game->legal_moves(moves);
  for (unsigned int i = 0; i < children.size(); i++) {
    // empty node
    //if (children[i] == nullptr) {
    //  continue;
    //}

    unsigned int sum_n_visited = this->n_visited.load() + 1;
    double cur_value =
        children[i]->get_value(c_puct, c_virtual_loss, sum_n_visited);
    if (cur_value > best_value) {
      best_value = cur_value;
      //game->legal_moves(moves);
      best_move = i;
     // best_node = children[i];
    }
  }
  //LOG(ERROR) << "select end";
  //double best_value = -DBL_MAX;
  // add vitural loss
  //LOG(ERROR)<<"v _loss ++++";
  if(best_move<moves.size()){
    children[best_move]->virtual_loss++;
  }
  //LOG(ERROR)<<"best_move:"<<best_move;
  return best_move;
}

void TreeNode::expand(const std::vector<float> &action_priors,const bool& full_search,unsigned int& node_num){
  {
    // get lock
    std::lock_guard<std::mutex> lock(this->lock);
    //LOG(ERROR)<<"expand";
    if (this->is_leaf) {
      //unsigned int action_size = this->children.size();

      for (unsigned int i = 0; i < action_priors.size(); i++) {
        // illegal action
        //if (abs(action_priors[i] - 0) < FLT_EPSILON) {
         // continue;
        //}
          //std::vector<Move> moves;
          //auto game_copy = std::make_shared<GameState>(*game);
          //game_copy->apply_move(legal_moves[i], *game_copy);
          //game_copy->legal_moves(moves);
       // if(!full_search){
            //LOG(ERROR)<<full_search;

            //LOG(ERROR)<<"node num:"<<node_num;
           // if(node_num<300){
              //  this->children.push_back(new TreeNode(this, action_priors[i]));
                //LOG(ERROR)<<"children:"<<this->children.size();
             //   node_num++;
           // }else{
                // node > 300
        //        LOG(ERROR)<<"node > 300 ";
            //    break;
          //  }
        //}else{
            this->children.push_back(new TreeNode(this, action_priors[i]));
        //}
      }
        //LOG(ERROR)<<"end loop";
      // not leaf
      //if(this->children.size()>0){
        this->is_leaf = false;
        //LOG(ERROR)<<"children size:"<<this->children.size();
        return;
      //}else{
        //this->is_leaf = true;
        //LOG(ERROR)<<"children size:"<<this->children.size();
       // return;
     // }

      //LOG(ERROR)<<"children size:"<<this->children.size();
    }
    //LOG(ERROR)<<"end expand";
  }
  //LOG(ERROR)<<"expand end";
}

void TreeNode::backup(double value) {
  // If it is not root, this node's parent should be updated first
  //
  //std::cout<<"vlaue:"<<value<<std::endl;
  //LOG(ERROR)<<"backup start";
  if (this->parent != nullptr) {
   this->parent->backup(-value);
  }

  // remove vitural loss
  this->virtual_loss--;

  // update n_visited
  //unsigned int n_visited = this->n_visited.load();

  //std::cout<<"visit:"<<n_visited;
  //this->n_visited++;

  //std::cout<<"visit:"<<this->n_visited;
  //this->n_visited++;
  //std::cout<<"visit:"<<this->n_visited;


  // update q_sa
  {
    std::lock_guard<std::mutex> lock(this->lock);
    this->q_sa = (n_visited * this->q_sa + value) / (n_visited + 1);
  }
  n_visited++;
  //std::cout<<"visit:"<<n_visited;


  //LOG(ERROR)<<"backup end";
}

double TreeNode::get_value(double c_puct, double c_virtual_loss,
                           unsigned int sum_n_visited) const {
  // u
  auto n_visited = this->n_visited.load();
  double u = (c_puct * this->p_sa * sqrt(sum_n_visited) / (1 + n_visited));

  // virtual loss
  //
  //LOG(ERROR)<<"this loss:"<<this->virtual_loss.load();
  double virtual_loss = c_virtual_loss * this->virtual_loss.load();
  // int n_visited_with_loss = n_visited - virtual_loss;

  if (n_visited <= 0) {
    return u;
  } else {
    return u + (this->q_sa * n_visited - virtual_loss) / n_visited;
  }
}

// MCTS
MCTS::MCTS(deep_model *neural_network, unsigned int thread_num, double c_puct,
           unsigned int num_mcts_sims, double c_virtual_loss)
    : neural_network(neural_network),
      thread_pool(new ThreadPool(thread_num)),
      c_puct(c_puct),
      num_mcts_sims(num_mcts_sims),
      c_virtual_loss(c_virtual_loss),
      //action_size(action_size),
      root(new TreeNode(nullptr, 1.), MCTS::tree_deleter),
      node_num(0){

      //q = TaskQueue();  // 使用 4 个工作线程

    //auto future_result = queue.enqueue([&model, &grid]() {
    //    return model.commit(grid);  // 假设 commit 现在返回值而非 future
    //});
    }

MCTS::MCTS(unsigned int thread_num, double c_puct,
           unsigned int num_mcts_sims, double c_virtual_loss)
        : thread_pool(new ThreadPool(thread_num)),
          c_puct(c_puct),
          num_mcts_sims(num_mcts_sims),
          c_virtual_loss(c_virtual_loss),
          //action_size(action_size),
          root(new TreeNode(nullptr, 1.), MCTS::tree_deleter),
          node_num(0){}

void MCTS::update_with_move(int last_action) {
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

void MCTS::tree_deleter(TreeNode *t) {
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

std::vector<float> MCTS::get_action_probs(GameState *game, const bool& show,const bool& full_search, double temp) {
  // submit simulate tasks to thread_pool
  std::vector<std::future<void>> futures;
  if(show){
    LOG(ERROR)<<"full_search:"<<full_search;
  }
  this->node_num=0;
  //auto mcts_start = std::chrono::high_resolution_clock::now();
  //LOG(ERROR)<<"start simulate";
  for (unsigned int i = 0; i < this->num_mcts_sims; i++) {
    // copy gomoku
    auto game_copy = std::make_shared<GameState>(*game);
    //auto game_copy = game;
    auto future =
        this->thread_pool->commit(std::bind(&MCTS::simulate, this, game_copy, full_search,this->node_num));
   // simulate(game_copy,full_search,this->node_num);
    // future can't copy
    futures.emplace_back(std::move(future));
  }

  // wait simulate
  for (unsigned int i = 0; i < futures.size(); i++) {
    futures[i].wait();
 }




        //auto mcts_start = std::chrono::high_resolution_clock::now();

        //auto probs = mcts.get_action_probs(&game_copy,5);

        //if(show){
            //LOG(ERROR)<<"game state length:"<<states.size();

   // auto mcts_end = std::chrono::high_resolution_clock::now();
            // 计算时间间隔
    //std::chrono::duration<double> duration = mcts_end - mcts_start;
            // 输出时间间隔
   //LOG(ERROR) << "simulate speed time " << duration.count() << "s";
        //}






  //LOG(ERROR)<<"end simulate";

  // calculate probs
  std::vector<Move> s;
  game->legal_moves(s);
  std::vector<float> action_probs(s.size(), 0);
  const auto &children = this->root->children;
  //LOG(ERROR)<<"root visit:"<<this->root->n_visited.load();
  // greedy
  //std::cout<<temp<<std::endl;

  //auto t = temp-1e-3;

  //std::cout<<temp<<std::endl;

  if (temp - 1e-3 < FLT_EPSILON) {
    unsigned int max_count = 0;
    unsigned int best_action = 0;

    for (unsigned int i = 0; i < children.size(); i++) {
      if (children[i] && children[i]->n_visited.load() > max_count) {
        max_count = children[i]->n_visited.load();
        best_action = i;
      }
    }

    action_probs[best_action] = 1.;
    return action_probs;

  } else {
    // explore
    //
   // std::cout<<"run else "<<std::endl;
    double sum = 0;

    //std::cout<<children.size()<<std::endl;

    for (unsigned int i = 0; i < children.size(); i++) {
        if(show){
            LOG(ERROR)<<"child visit:"<<children[i]->n_visited.load()<<"\t";
        }
      if (children[i] && children[i]->n_visited.load() > 0) {
          //std::cout<<children[i]->n_visited.load()<<std::endl;
        action_probs[i] = pow(children[i]->n_visited.load(), 1 / temp);

        //std::cout<<"action_pro:"<<action_probs[i]<<"\t";

        sum += action_probs[i];
      }
    }
    //std::cout<<std::endl;
   // std::cout<<sum<<std::endl;
    // renormalization
    std::for_each(action_probs.begin(), action_probs.end(),
                  [sum](float &x) { x /= sum; });

    return action_probs;
  }
}

void MCTS::simulate(std::shared_ptr<GameState> game,const bool& full_search,unsigned int& node_num) {
  // execute one simulation
  auto node = this->root.get();
//  std::cout<< "simulation"<<std::endl;

  //LOG(ERROR)<<node->get_is_leaf();
  int w=0;

  std::vector<Move> moves;
  //{
  //std::lock_guard<std::mutex> lock(this->lock);
  while (true) {
    //{
    //std::lock_guard<std::mutex> lock(this->lock);
    //w=0;
    //game->is_gameover(w);
    //
    //{
    //std::lock_guard<std::mutex> lock(this->lock);
    moves.clear();
    game->legal_moves(moves);
    if (node->get_is_leaf()) {
      break;
    }else if(moves.size() == 0){
        game->move.m_point = -100;
        game->move.m_point_ = -100;
        break;
    }
    //}

    //LOG(ERROR)<<"select start";
    auto action = node->select(game,this->c_puct, this->c_virtual_loss);
    //LOG(ERROR)<<"select end";
    //GameState game;
    //LOG(ERROR)<<"move index:"<<action;
    //{
    //std::lock_guard<std::mutex> lock(this->lock);
    //if(action != -DBL_MAX){
        //LOG(ERROR)<<"apply move----------------------------";
       // moves.clear();
        //game->legal_moves(moves);
        //LOG(ERROR)<<game;
        //if(moves.size() == node->children.size() && game != nullptr){
            game->apply_move(moves[action], *game);
             //LOG(ERROR)<<"move:"<<moves.size();
             //LOG(ERROR)<<"node children:"<<node->children.size();
            //if(node->children.size()>action){
                node = node->children[action];
           //}else{
            //    LOG(ERROR)<<"action:"<<action;
            //    LOG(ERROR)<<"node children:"<<node->children.size();
            //    LOG(ERROR)<<"!!!!!!!!!!!!!!error action:"<<action;
            //    print_board(game->board);
            //    break;
           // }
        //}else{
        //    LOG(ERROR)<<"!!!!!!!!!!!!!!game is nullptr"<<action;
        //    break;
       // }
        //LOG(ERROR)<<"apply move end -----------------------------";
   //}else{
   //  LOG(ERROR)<<"!!!!!!!!!!!!!!error action:"<<action;
   //  break;
  // }
   //}
    //LOG(ERROR)<<"apply_move end";
  }
  //std::cout<<"to leaf"<<std::endl;

  // get game status
  int winner = 0;
  game->is_gameover(winner);
  float value = 0;

  // not end
  if (winner == 0) {
    // predict action_probs and value by neural network
    std::vector<float> action_priors(64, 0.);

    float grid[81*5]={0};

    encoder_data(game, grid);

    //for(auto i:grid){
    //    std::cout<<i<<"\t";
    //}
    //std::cout<<"pre infer"<<std::endl;

  //  if(!fileExists("/home/dev/kingchess_remote/kingchess/net/model/current.engine")){
    //    std::this_thread::sleep_for(std::chrono::seconds(30));
    //}
    //{
    //std::lock_guard<std::mutex> lock(this->lock);
    //deep_model model("/home/dev/kingchess_remote/kingchess/net/model/current.engine");

    //auto start = std::chrono::high_resolution_clock::now();
    //deep_model model(FLAGS_engine_path);

    //auto inferstart = std::chrono::high_resolution_clock::now();
    //deep_model model("/home/test4/tmp/kingchess/net/model/current.engine");
    // std::cout<<"model init"<<std::endl;
    //auto result = model.commit(grid);

    //LOG(ERROR)<<"start infer";
    auto result = neural_network->commit(grid);
    //
    //

    //auto future_result = q.enqueue([this->neural_network,grid]() {
    //    return neural_network.commit(grid);
    //});

    //auto result = future_result.get();

    // speed time infer
    //

    //auto end = std::chrono::high_resolution_clock::now();
            // 计算时间间隔
    //std::chrono::duration<double> duration = end - inferstart;
            // 输出时间间隔

    //std::chrono::duration<double> duration_init = inferstart - start;
    //LOG(ERROR) << "init_model speed time " << duration_init.count() << "s";

    //LOG(ERROR) << "infer speed time " << duration.count() << "s";
    //}
    //auto result =  neural_network->commit(grid);
    //LOG(ERROR)<<"end infer";


    action_priors = std::move(result[0]);
    value = result[1][0];

//    for(auto prob:action_priors){
//        std::cout<<prob<<"\t";
//    }
    //std::cout<<value<<std::endl;

//    std::cout<< value<<std::endl;

    // mask invalid actions
    std::vector<Move> legal_move;
    game->legal_moves(legal_move);

    action_priors.resize(legal_move.size());


    //std::cout<<action_priors.size()<<std::endl;

    //std::cout<<legal_move.size()<<std::endl;

    //assert(action_priors.size()==legal_move.size());

   // std::cout<<action_priors.size()<<std::endl;

    //for(auto it:action_priors){
    //    std::cout<<it<<"\t";
    //}
    //std::cout<<std::endl;


    double sum = 0;
    for (unsigned int i = 0; i < action_priors.size(); i++) {
        sum += action_priors[i];
    }
    //std::cout<<sum<<std::endl;

    // renormalization
    if (sum > FLT_EPSILON) {
      std::for_each(action_priors.begin(), action_priors.end(),
                    [sum](float &x) { x /= sum; });
    } else {

        //std::cout<< "run else"<<std::endl;
      // all masked

      // NB! All valid moves may be masked if either your NNet architecture is
      // insufficient or you've get overfitting or something else. If you have
      // got dozens or hundreds of these messages you should pay attention to
      // your NNet and/or training process.
//      std::cout << "All valid moves were masked, do workaround." << std::endl;

      for (unsigned int i = 0; i < action_priors.size(); i++) {
          //if(i<legal_move.size()) {
         action_priors[i] = 1.0f / static_cast<float>(action_priors.size());
          }
      }
    //}


    // expand
    //LOG(ERROR)<<"enter expand";
    node->expand(action_priors,full_search,node_num);
   // LOG(ERROR)<<"expand run end";


  } else {
    // end
//    auto winner = status[1];
    value = (winner == game->player ? 1 : -1);
  }

  // value(parent -> node) = -value
  //LOG(ERROR)<<"enter backup function";
  node->backup(-value);

//  LOG(ERROR)<<"root visited:"<<root->n_visited.load();
  return;
}
