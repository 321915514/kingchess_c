//
// Created by dev on 4/9/24.
//

#ifndef KINGCHESS_COLLECT_H
#define KINGCHESS_COLLECT_H

#include "kingchess_c/net/mcts_45_expert.h"
#include "kingchess_c/net/deep_model_45.h"
#include <vector>
#include <algorithm>
#include <random>
#include "kingchess_c/fundamental/train.pb.h"
//#include "thirdparty/glog/logging.h"

//struct game_state_prob{
//    float* state;
//    std::pair<std::string,float> prob;
//    int player;
//};

class Collect{
public:
    //int iters;
    double c_puct = 5;
    int simulate_num;
    int thread_num;

    deep_model model;

    Collect(const double& temp,const int& simulate_num,const int& thread_num );

    void self_play(const bool& show);

    int find_max_index(const std::vector<float>& probs){
        auto maxIt = std::max_element(probs.begin(), probs.end());
        return std::distance(probs.begin(),maxIt);
    }

    bool full_search(){
        std::random_device rd;
        std::mt19937 gen(rd());
        std::bernoulli_distribution dis(0.75);

        if(dis(gen)){
            return true;
        }else{
            return false;// false == 0.25
        }

    }


#include <vector>
#include <random>
#include <algorithm>

int combineProbabilities(const std::unordered_map<int,double>& actions) {
    std::vector<int> acts;
    
    std::vector<double> probs;

    // 0.75 * probs
    for (auto pair:actions) {
        acts.push_back(pair.first);
	probs.push_back(pair.second);
    }

    // 0.25 * np.random.dirichlet(0.6 * np.ones(len(probs)))
    std::random_device rd;
    std::mt19937 gen(rd());
    std::vector<double> dirichletValues(actions.size());
    std::vector<double> ones(actions.size(), 0.6);
    std::gamma_distribution<double> gammaDist(0.6);

    // Generate Dirichlet noise for each legal action
    for (size_t i = 0; i < probs.size(); ++i) {
        dirichletValues[i] = gammaDist(gen);
    }

    // Normalize the Dirichlet noise
    double total = std::accumulate(dirichletValues.begin(), dirichletValues.end(), 0.0);
    for (size_t i = 0; i < probs.size(); ++i) {
        dirichletValues[i] /= total;
    }

    // Add Dirichlet noise to the legal positions
    for (size_t i = 0; i < probs.size(); ++i) {
	probs[i] =  0.75*probs[i] + 0.25 * dirichletValues[i];
    }
    std::default_random_engine generator;
    std::discrete_distribution<int> dist(probs.begin(),probs.end());

    return acts[dist(generator)];

}



//    std::vector<std::tuple<std::array<float,81*5>,std::vector<float>,int>> get_equi_data(std::vector<std::array<float,81*5>> states,  std::vector<std::vector<float>> move_probs, std::vector<int> current_players);

//    std::vector<std::tuple<std::array<float,81*5>,std::vector<float>,int>> symmetry(std::array<float, 81*5>& state, std::vector<float>& move_prob,const int& current_player);

    void write_proto_file(std::vector<std::tuple<std::array<float,45*32>,std::vector<float>,int>>& play_data,const bool& show);

};





#endif //KINGCHESS_COLLECT_H
