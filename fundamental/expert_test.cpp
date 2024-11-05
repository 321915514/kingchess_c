//
// Created by 32191 on 2024/8/23.
//
#include "expert.h"

#include "utils.h"
//#include "agent.h"

int play(){

    GameState game;
    GameState::new_game(5,9,game);

    Move m;
    int winner = 0;
    while(true) {
        print_board(game.board);
	if (game.moves.size()>32){
            std::cout<<Move::move2str(game.moves.back())<<"----------:"<<Move::move2str(game.moves[game.moves.size()-5])<<std::endl;
	}
        game.is_gameover(winner);
        if(winner!=0) {
            if(winner == BLACK) {
                // std::cout<<"black"<<std::endl;
                return BLACK;
            }
            if(winner == WHITE) {
                // std::cout<<"white"<<std::endl;
                return WHITE;
            }

        }

        if(game.player == BLACK) {
            //Bot::select_move(game,m);
            //game.apply_move(m,game);
	    //
     	    auto moves = Expert_agent::select_move(game);
            // for(auto item:moves) {
            //     std::cout<<Move::move2str(item.first) <<":"<<item.second<<"\t";
            // }
            // std::cout<<std::endl;

            auto maxElement = std::max_element(moves.begin(), moves.end(),
                                        [](const auto& a, const auto& b) {
                                            return a.second < b.second;
                                        });
            // print_move(maxElement->first);
            game.apply_move(maxElement->first,game);

        }else {
            auto moves = Expert_agent::select_move(game);
            // for(auto item:moves) {
            //     std::cout<<Move::move2str(item.first) <<":"<<item.second<<"\t";
            // }
            // std::cout<<std::endl;

            auto maxElement = std::max_element(moves.begin(), moves.end(),
                                        [](const auto& a, const auto& b) {
                                            return a.second < b.second;
                                        });
            // print_move(maxElement->first);
            game.apply_move(maxElement->first,game);
        }
        // auto index = get_random_index(moves.size());
        // auto it = moves.begin();
        // // 移动迭代器到随机位置
        // std::advance(it, index);


    }

}


int main(){
    int black =0;
    int white = 0;
    // play();
    for(int i=0;i<1;i++) {
	    auto result = play();
        if(result==BLACK) {
            black+=1;
        }
        if(result == WHITE) {
            white+=1;
        }
    }
    std::cout<<black<<" "<<white<<std::endl;
}
