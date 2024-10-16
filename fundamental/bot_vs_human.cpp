//
// Created by philxling on 2024/4/6.
//
#include "coordinate.h"
#include "gamestate.h"
#include "board.h"
#include "utils.h"
#include "agent.h"
#include "sstream"
#include <string>
void string2move(const std::string& userInput,Move &move){
    std::string cols = "ABCDEFGHI";
    if(userInput.size() == 2){
//        std::cout<< userInput[1]<<std::endl;
        int row = userInput[1]-'0';
        int col = static_cast<int>(cols.find(userInput[0]));
        auto coord = Coordinate::Coord(col,row);
        move.m_point = coord;
    }else{
        std::istringstream iss(userInput);
        std::vector<std::string> words;
        std::string word;

        // 使用 getline 逐个读取单词
        while (iss >> word) {
            words.push_back(word);
        }
        auto m_point_row = words[0][1]-'0';
        auto m_point_col = static_cast<int16_t>(cols.find(words[0][0]));
        auto coord = Coordinate::Coord(m_point_col,m_point_row);
        auto m_point_row_ = words[1][1]-'0';
        auto m_point_col_ = static_cast<int16_t>(cols.find(words[1][0]));
        auto coord_ = Coordinate::Coord(m_point_col_,m_point_row_);
        move.m_point = coord;
        move.m_point_ = coord_;
    }
}



void run(){
    GameState game;
    GameState::new_game(5,9,game);
    while (true){
        print_board(game.board);
        std::cout<<"play_out:"<<game.play_out<<std::endl;
        if(game.eat_chess()>=11){
            std::cout<<"black win !!"<<std::endl;
            break;
        }
        Move move;
        if(game.player == BLACK){
            Bot::select_move(game,move);
            if(move.m_point == -100 && move.m_point_ == -100){
                std::cout<<"white win !!"<<std::endl;
                break;
            }
            print_move(game.player,move);


        } else{


            std::string userInput;

            std::cout << "--";
            getline(std::cin, userInput);

            string2move(userInput,move);

//            Bot::select_move(game,move);

            print_move(game.player,move);
//            std::cout<<"play_out:"<<game.play_out<<std::endl;
        }
        game.apply_move(move,game);

    }
}


int main(){
    run();
}