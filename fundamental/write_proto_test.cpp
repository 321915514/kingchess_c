//
// Created by philxling on 2024/4/13.
//
#include "kingchess_c/fundamental/train.pb.h"
#include "gamestate.h"
#include "utils.h"
#include <fstream>

int main(){
    GameState gameState;
    GameState::new_game(5,9,gameState);

    float grid[81*5];
    encoder_data(std::make_shared<GameState>(gameState),grid);

    TripletArray triplet_array;

    Triplet triplet = Triplet();

    for(auto item:grid){
        triplet.add_array(static_cast<int32_t>(item));
    }
    Stone move;
    move.set_point("ewew");
    move.set_prob(12.657);
    *triplet.add_dictionary() = move;
    triplet.set_value(-1);
    *triplet_array.add_triplets() = triplet;


    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    char filename[80];
    sprintf(filename, "file_%d-%02d-%02d_%02d:%02d:%02d.game",
            tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
            tm.tm_hour, tm.tm_min, tm.tm_sec);
    printf("Current time: %s\n", filename);

    std::string file(filename);
    std::string filename1 = "/home/test4/kingchess_remote/"+file;
    // 将消息序列化到文件
    std::fstream out(filename1, std::ios::out | std::ios::binary | std::ios::trunc);
    if (!triplet_array.SerializeToOstream(&out)) {/// SerializeToOstream
        std::cerr << "Failed to write triplet array." << std::endl;
        return 0;
    }
    std::cout <<filename<< " write." << std::endl;



}

