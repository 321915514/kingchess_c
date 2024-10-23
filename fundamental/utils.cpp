//
// Created by wangxu on 2023/8/7.
//

#include "utils.h"
#include<string>
#include "coordinate.h"
#include "board.h"
#include <iostream>
#include <ctime>
#include <thread>
#include <fstream>

const std::string COLS = "ABCDEFGHIJKLMNOPQRST";

//Point point_from_coords(const std::string& coords) {
//    int col = COLS.find(coords[0]) + 1;
//    int row = std::stoi(coords.substr(1));
//    return Point(row, col);
//}

void print_move(coord_t player, const Move& move) {
    std::string str = player == BLACK ? "black" : "white";
    std::string move_str;
    if(Move::is_down(move)){
        move_str += "point(row="+std::to_string(std::get<1>(Coordinate::GetXY(move.m_point)))+",col="+std::to_string(std::get<0>(Coordinate::GetXY(move.m_point)))+")";
        std::cout << str << " " << move_str << std::endl;
    } else if(Move::is_go(move)){
        move_str += "point(row="+std::to_string(std::get<1>(Coordinate::GetXY(move.m_point)))+",col="+std::to_string(std::get<0>(Coordinate::GetXY(move.m_point)))+")"
                +"--------point(row="+std::to_string(std::get<1>(Coordinate::GetXY(move.m_point_)))+",col="+std::to_string(std::get<0>(Coordinate::GetXY(move.m_point_)))+")";
        std::cout << str << " " << move_str << std::endl;
    }
}



//void print_board(const Board& board){
//
//    for (int row = Coordinate::m_size-1; row >= 0; --row) {
//        std::string bump = (row <= 9) ? " " : "";
//        std::string line;
//        for (int col = 0; col < Coordinate::m_size; ++col) {
//            auto stone = board.grid[Coordinate::Coord(col,row)];
//            if (stone == BLACK) {
//                line += "X";
//            }
//            else if (stone == WHITE) {
//                line += "O";
//            }
//            else {
//                line += ".";
//            }
//            line += "   ";
//        }
//        std::cout << bump << row << " " << line << std::endl;
//    }
//    std::cout << "   ";
//    for (int col = 0; col < Coordinate::m_size; ++col) {
//        std::cout << COLS[col] << "   ";
//    }
//    std::cout << std::endl;
//
//}

void print_board(const Board& board){
    for(int16_t row=0;row<5;row++){
        std::string bump = (row <= 9) ? " " : "";
        std::string line;
        for(int16_t col=0;col<9;col++){
            auto stone = board.grid[Coordinate::Coord(col,row)];
            if (stone == BLACK) {
                line += "X";
            }
            else if (stone == WHITE) {
                line += "O";
            }
            else if(!Coordinate::Legal_coord(Coordinate::Coord(col,row))) {
                line += " ";
            }else {
                line += ".";
            }
            line += "   ";
        }
        std::cout << bump << row << " " << line << std::endl;
    }
    std::cout << "   ";
    for (int col = 0; col < Coordinate::m_size; ++col) {
        std::cout << COLS[col] << "   ";
    }
    std::cout << std::endl;
}


coord_t get_random_index(size_t max) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, static_cast<int>(max) - 1);
    int index = dis(gen);

    return static_cast<int16_t>(index);
//    srand(unsigned(time(NULL)));
//    std::this_thread::sleep_for(std::chrono::nanoseconds);
//    if(max>0){
//        return static_cast<int16_t>(rand()%(max));
//    }
}

void print_move(const Move& move) {
    std::string move_str;
    if(Move::is_down(move)){
        move_str += "point(row="+std::to_string(std::get<1>(Coordinate::GetXY(move.m_point)))+",col="+std::to_string(std::get<0>(Coordinate::GetXY(move.m_point)))+")";
        std::cout << move_str << std::endl;
    } else if(Move::is_go(move)){
        move_str += "point(row="+std::to_string(std::get<1>(Coordinate::GetXY(move.m_point)))+",col="+std::to_string(std::get<0>(Coordinate::GetXY(move.m_point)))+")"
                    +"--------point(row="+std::to_string(std::get<1>(Coordinate::GetXY(move.m_point_)))+",col="+std::to_string(std::get<0>(Coordinate::GetXY(move.m_point_)))+")";
        std::cout << move_str << std::endl;
    }
}

void encoder_data(std::shared_ptr<GameState> game, float (&grid)[81*5]){
    for(size_t i=0;i<SQUARE;i++){
        if(game->board.grid[i] == 1){
            grid[i] = 1;
        }else if(game->board.grid[i] == -1){
            grid[i+81] = 1;
        }

        for(size_t j=81*2;j<81*3;j++){
            grid[j] = game->player==BLACK?1:-1;
        }

        if(Move::is_down(game->move)){
            grid[game->move.m_point+(81*3)] = 1;
        }else if(Move::is_go(game->move)){
            grid[game->move.m_point+(81*3)] = -1;
            grid[game->move.m_point_+(81*3)] = 1;
        }
        if(game->eat_point != -1) {
            grid[(81 * 4) + game->eat_point] = 1;
        }
    }
//    if(game->play_out<33){
//    	int layer =  (game->play_out)/2;
//    	for(size_t i=0;i<SQUARE;i++){
//    	   grid[(81*5+81*layer)+i] = 1; 
//    	}
//    }
}


void encoder_data_45(std::shared_ptr<GameState> game, float (&grid)[45*32]){
    for(size_t i=0;i<SQUARE;i++){
        if(game->board.grid[i] == 1){
            grid[i] = 1;
        }else if(game->board.grid[i] == -1){
            grid[i+45] = 1;
        }

        for(size_t j=45*2;j<45*3;j++){
            grid[j] = game->player==BLACK?1:-1;
        }

	//if(Move::is_down(game->move)){
        //    grid[game->move.m_point+(45*3)] = 1;
        //}else if(Move::is_go(game->move)){
        //    grid[game->move.m_point+(45*3)] = -1;
        //    grid[game->move.m_point_+(45*3)] = 1;
        //}
       // if(game->eat_point != -1) {
       //     grid[(45 * 4) + game->eat_point] = 1;
       // }
    }
    if(game->play_out<33){
    	int layer =  (game->play_out)/2;
    	for(size_t i=0;i<SQUARE;i++){
    	   grid[(45*3+45*layer)+i] = 1; 
    	}
    }

    for(size_t i=0;i<SQUARE;i++){
    	 grid[(45*20+45*(game->eat_chess()))+i] = 1; 
    }







}




void encoder_board(std::shared_ptr<GameState> game, float (&grid)[81]){
    for(size_t i=0;i<SQUARE;i++){
        if(game->board.grid[i] == 1){
            grid[i] = 1;
        }else if(game->board.grid[i] == -1){
            grid[i] = -1;
        }
    }
}

