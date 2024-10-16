//
// Created by wangxu on 2023/10/17.
//
#include <iostream>
#include <cstdint>
#include <deque>
#include "neighbor.h"
#include "coordinate.h"
#include "gamestate.h"
#include "utils.h"
#include <chrono>
#include <iomanip>

void parse_points(const coord_t& coord,std::map<coord_t,std::vector<coord_t>>& jump_dict, std::vector<std::vector<coord_t>>& v_moves){
    std::deque<coord_t> stack;
    stack.push_back(coord);
    std::vector<coord_t> path_st;
    std::vector<coord_t> result;
    while (!stack.empty()){
        coord_t cur = stack[stack.size()-1];
        if (cur == -1) {
            stack.pop_back();
            path_st.pop_back();
            continue;
        }else{
            stack.pop_back();
            path_st.push_back(cur);
        }
        if (jump_dict.find(cur)==jump_dict.end()){
            std::vector<coord_t> pa;
            pa.assign(path_st.begin(), path_st.end());
            v_moves.push_back(pa);
            path_st.pop_back();
        }
        if (jump_dict.find(cur)!=jump_dict.end()){
            std::vector<coord_t> cur_child = jump_dict[cur];
            cur_child.push_back(-1);
            reverse(cur_child.begin(),cur_child.end());
            for(auto i:cur_child){
                stack.push_back(i);
            }
        }


    }
    jump_dict.clear();
}


std::string TimeString(double time_in_second) {
    std::ostringstream oss;
    oss << std::setfill('0')
        << std::setw(2) << static_cast<int>(time_in_second) / 60 << ":"
        << std::setw(2) << static_cast<int>(time_in_second) % 60;
    return oss.str();
}




int main(){

//    std::cout << static_cast<char>('0' + 26) ;


    std::cout<<get_random_index(2);


    std::cout<<std::endl;

    GameState game;
    GameState::new_game(8,game);

    int a[10] = {0};

    std::cout<<Coordinate::Coord(0,0)<<std::endl;




    for(coord_t coord:Neighbor8(28)){
        std::cout<<coord<<"\t";

    }
    std::cout<<std::endl;
    std::vector<coord_t> neighbors;
    neighbors.assign(Neighbor8(28).begin(),Neighbor8(28).end());

    neighbors.push_back(neighbors[0]);

    for(coord_t i =2;i<neighbors.size();i+=2){
        std::cout<<neighbors[i-2]<<"\t"<<neighbors[i-1]<<"\t"<<neighbors[i]<<std::endl;
    }

//    for(coord_t coord:neighbors){
//        std::cout<<coord<<"\t";
//    }



    std::cout<<std::endl;
    std::cout<< Coordinate::m_line[9]<<std::endl;


    std::cout<<std::endl;



    for(coord_t coord:DiagNeighbor4 (28)){
        std::cout<<coord<<"\t";
    }
    std::cout<<std::endl;
    for(coord_t coord:DiagNeighbor4 (35)){
        std::cout<<coord<<"\t";
    }
    std::cout<<std::endl;


    std::cout<<"test"<<std::endl;


    std::map<coord_t,std::vector<coord_t>> jump_dict;

    std::vector<coord_t> tmp1;
    tmp1.push_back(2);
    tmp1.push_back(3);


    std::vector<coord_t> tmp2;
    tmp2.push_back(4);
    tmp2.push_back(5);

    std::vector<coord_t> tmp5;
    tmp5.push_back(4);


    jump_dict[1]=tmp1;
    jump_dict[2]=tmp2;
    jump_dict[5]=tmp5;

    std::vector<std::vector<coord_t>> v_moves;
    parse_points(1,jump_dict,v_moves);


    for(const auto& item:v_moves){
        for(auto coord:item){
            std::cout<<coord<<"-\t";
        }
        std::cout<<std::endl;
    }

    auto now = std::chrono::system_clock::now();

    std::cout<<now.time_since_epoch().count()/1000000- 1000 * 20<<std::endl;




}