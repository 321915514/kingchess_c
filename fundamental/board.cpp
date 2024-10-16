//
// Created by wangxu on 2023/8/7.
//

#include <tuple>
#include <set>
#include "board.h"
#include "utils.h"



const std::set<std::pair<coord_t, coord_t>> Board::invalid_moves = {

//        {Coordinate::Coord(3,0),Coordinate::Coord(2,1)},
//        {Coordinate::Coord(2,1),Coordinate::Coord(3,0)},
//        {Coordinate::Coord(3,2),Coordinate::Coord(2,1)},
//        {Coordinate::Coord(2,1),Coordinate::Coord(3,2)},
//        {Coordinate::Coord(3,2),Coordinate::Coord(2,3)},
//        {Coordinate::Coord(2,3),Coordinate::Coord(3,2)},
//
//        {Coordinate::Coord(3,4),Coordinate::Coord(2,3)},
//        {Coordinate::Coord(2,3),Coordinate::Coord(3,4)},
//
//        {Coordinate::Coord(3,0),Coordinate::Coord(4,1)},
//        {Coordinate::Coord(4,1),Coordinate::Coord(3,0)},
//        {Coordinate::Coord(3,2),Coordinate::Coord(4,1)},
//        {Coordinate::Coord(4,1),Coordinate::Coord(3,2)},
//
//        {Coordinate::Coord(3,2),Coordinate::Coord(4,3)},
//        {Coordinate::Coord(4,3),Coordinate::Coord(3,2)},
//
//        {Coordinate::Coord(3,4),Coordinate::Coord(4,3)},
//        {Coordinate::Coord(4,3),Coordinate::Coord(3,4)},
//
//        {Coordinate::Coord(5,0),Coordinate::Coord(4,1)},
//        {Coordinate::Coord(4,1),Coordinate::Coord(5,0)},
//        {Coordinate::Coord(5,2),Coordinate::Coord(4,1)},
//        {Coordinate::Coord(4,1),Coordinate::Coord(5,2)},
//
//
//
//        {Coordinate::Coord(5,2),Coordinate::Coord(4,3)},
//        {Coordinate::Coord(4,3),Coordinate::Coord(5,2)},
//        {Coordinate::Coord(5,4),Coordinate::Coord(4,3)},
//        {Coordinate::Coord(4,3),Coordinate::Coord(5,4)},
//
//        {Coordinate::Coord(5,0),Coordinate::Coord(6,1)},
//        {Coordinate::Coord(6,1),Coordinate::Coord(5,0)},
//        {Coordinate::Coord(5,2),Coordinate::Coord(6,1)},
//        {Coordinate::Coord(6,1),Coordinate::Coord(5,2)},
//
//        {Coordinate::Coord(5,2),Coordinate::Coord(6,3)},
//        {Coordinate::Coord(6,3),Coordinate::Coord(5,3)},
//
//        {Coordinate::Coord(5,4),Coordinate::Coord(6,3)},
//        {Coordinate::Coord(6,3),Coordinate::Coord(5,4)},
//
//        {Coordinate::Coord(3,0),Coordinate::Coord(5,2)},
//        {Coordinate::Coord(5,2),Coordinate::Coord(3,0)},
//        {Coordinate::Coord(4,1),Coordinate::Coord(6,3)},
//        {Coordinate::Coord(6,3),Coordinate::Coord(4,1)},
//
//        {Coordinate::Coord(2, 1), Coordinate::Coord(4, 3)},
//        {Coordinate::Coord(4, 3), Coordinate::Coord(2, 1)},
//        {Coordinate::Coord(3, 2), Coordinate::Coord(5, 4)},
//        {Coordinate::Coord(5, 4), Coordinate::Coord(3, 2)},
//
//        {Coordinate::Coord(5, 0), Coordinate::Coord(3, 2)},
//        {Coordinate::Coord(3, 2), Coordinate::Coord(5, 0)},
//        {Coordinate::Coord(4, 1), Coordinate::Coord(2, 3)},
//        {Coordinate::Coord(2, 3), Coordinate::Coord(4, 1)},
//
//        {Coordinate::Coord(6, 1), Coordinate::Coord(4, 3)},
//        {Coordinate::Coord(4, 3), Coordinate::Coord(6, 1)},
//        {Coordinate::Coord(5, 2), Coordinate::Coord(3, 4)},
//        {Coordinate::Coord(3, 4), Coordinate::Coord(5, 2)}

        {3, 11},
        {11, 3},
        {21, 11},
        {11, 21},
        {21, 29},
        {29, 21},

        {39, 29},
        {29, 39},

        {3, 13},
        {13, 3},
        {21, 13},
        {13, 21},

        {21, 31},
        {31, 21},

        {39, 31},
        {31, 39},

        {5, 13},
        {13, 5},
        {23, 13},
        {13, 23},

        {23, 31},
        {31, 23},
        {41, 31},
        {31, 41},

        {5, 15},
        {15, 5},
        {23, 15},
        {15, 23},

        {23, 33},
        {33, 32},

        {41, 33},
        {33, 41},

        {3, 23},
        {23, 3},
        {13, 33},
        {33, 13},

        {11, 31},
        {31, 11},
        {21, 41},
        {41, 21},

        {5, 21},
        {21, 5},
        {13, 29},
        {29, 13},

        {15, 31},
        {31, 15},
        {23, 39},
        {39, 23}


};


const std::set<std::pair<coord_t, coord_t>> Board::blocked_paths = {
//        {Coordinate::Coord(0, 2), Coordinate::Coord(1, 1)},
//        {Coordinate::Coord(1, 1), Coordinate::Coord(0, 2)},
//        {Coordinate::Coord(7, 1), Coordinate::Coord(8, 2)},
//        {Coordinate::Coord(8, 2), Coordinate::Coord(7, 1)},
//        {Coordinate::Coord(0, 2), Coordinate::Coord(1, 3)},
//        {Coordinate::Coord(1, 3), Coordinate::Coord(0, 2)},
//        {Coordinate::Coord(7, 3), Coordinate::Coord(8, 2)},
//        {Coordinate::Coord(8, 2), Coordinate::Coord(7, 3)},
//        {Coordinate::Coord(2, 0), Coordinate::Coord(1, 1)},
//        {Coordinate::Coord(1, 1), Coordinate::Coord(2, 0)},
//        {Coordinate::Coord(1, 1), Coordinate::Coord(3, 1)},
//        {Coordinate::Coord(3, 1), Coordinate::Coord(1, 1)},
//        {Coordinate::Coord(2, 1), Coordinate::Coord(1, 2)},
//        {Coordinate::Coord(1, 2), Coordinate::Coord(2, 1)},
//        {Coordinate::Coord(2, 1), Coordinate::Coord(1, 1)},
//        {Coordinate::Coord(1, 1), Coordinate::Coord(2, 1)},
//        {Coordinate::Coord(1, 3), Coordinate::Coord(2, 3)},
//        {Coordinate::Coord(2, 3), Coordinate::Coord(1, 3)},
//        {Coordinate::Coord(2, 3), Coordinate::Coord(1, 2)},
//        {Coordinate::Coord(1, 2), Coordinate::Coord(2, 3)},
//        {Coordinate::Coord(1, 3), Coordinate::Coord(2, 4)},
//        {Coordinate::Coord(2, 4), Coordinate::Coord(1, 3)},
//        {Coordinate::Coord(1, 3), Coordinate::Coord(3, 3)},
//        {Coordinate::Coord(3, 3), Coordinate::Coord(1, 3)},
//        {Coordinate::Coord(6, 0), Coordinate::Coord(7, 1)},
//        {Coordinate::Coord(7, 1), Coordinate::Coord(6, 0)},
//        {Coordinate::Coord(6, 1), Coordinate::Coord(7, 1)},
//        {Coordinate::Coord(7, 1), Coordinate::Coord(6, 1)},
//        {Coordinate::Coord(5, 1), Coordinate::Coord(7, 1)},
//        {Coordinate::Coord(7, 1), Coordinate::Coord(5, 1)},
//        {Coordinate::Coord(6, 1), Coordinate::Coord(7, 2)},
//        {Coordinate::Coord(7, 2), Coordinate::Coord(6, 1)},
//        {Coordinate::Coord(6, 3), Coordinate::Coord(7, 2)},
//        {Coordinate::Coord(7, 2), Coordinate::Coord(6, 3)},
//        {Coordinate::Coord(7, 3), Coordinate::Coord(5, 3)},
//        {Coordinate::Coord(5, 3), Coordinate::Coord(7, 3)},
//        {Coordinate::Coord(6, 3), Coordinate::Coord(7, 3)},
//        {Coordinate::Coord(7, 3), Coordinate::Coord(6, 3)},
//        {Coordinate::Coord(6, 4), Coordinate::Coord(7, 3)},
//        {Coordinate::Coord(7, 3), Coordinate::Coord(6, 4)},
//        {Coordinate::Coord(3, 0), Coordinate::Coord(1, 2)},
//        {Coordinate::Coord(1, 2), Coordinate::Coord(3, 0)},
//        {Coordinate::Coord(3, 4), Coordinate::Coord(1, 2)},
//        {Coordinate::Coord(1, 2), Coordinate::Coord(3, 4)},
//        {Coordinate::Coord(5, 0), Coordinate::Coord(7, 2)},
//        {Coordinate::Coord(7, 2), Coordinate::Coord(5, 0)},
//        {Coordinate::Coord(5, 4), Coordinate::Coord(7, 2)},
//        {Coordinate::Coord(7, 2), Coordinate::Coord(5, 4)},

        {18, 10},
        {10, 18},
        {16, 26},
        {26, 16},
        {18, 28},
        {28, 18},
        {34, 26},
        {26, 34},
        {2, 10},
        {10, 2},
        {10, 12},
        {12, 10},
        {11, 19},
        {19, 11},
        {11, 10},
        {10, 11},
        {28, 29},
        {29, 28},
        {29, 19},
        {19, 29},
        {28, 38},
        {38, 28},
        {28, 30},
        {30, 28},
        {6, 16},
        {16, 6},
        {15, 16},
        {16, 15},
        {14, 16},
        {16, 14},
        {15, 25},
        {25, 15},
        {33, 25},
        {25, 33},
        {34, 32},
        {32, 34},
        {33, 34},
        {34, 33},
        {42, 34},
        {34, 42},
        {3, 19},
        {19, 3},
        {39, 19},
        {19, 39},
        {5, 25},
        {25, 5},
        {41, 25},
        {25, 41}
};

bool Board::check_move1(const coord_t &point, const coord_t &point_) const{
    if (!Coordinate::OnBoard(point) || !Coordinate::OnBoard(point_) || !Coordinate::Legal_coord(point) || !Coordinate::Legal_coord(point_)){
        return false;
    }
    if (point == point_){
        return false;
    }
//    std::tuple<int16_t, int16_t> tup1 = Coordinate::GetXY(point);
//    std::tuple<int16_t, int16_t> tup2 = Coordinate::GetXY(point_);
//    int16_t si = std::get<0>(tup1);
//    int16_t sj = std::get<1>(tup1);
//    int16_t ei = std::get<0>(tup2);
//    int16_t ej = std::get<1>(tup2);

    //没有对角线的不让走 i 为列，j 为行
    if (invalid_moves.count(std::pair(point,point_))) {
        return false;
    }
    //限制某些没有线的地方不让走
    if(blocked_paths.count(std::pair(point,point_))) {
        return false;
    }
    // 距离，位置
    if(Coordinate::m_distance[point][point_] == sqrt(2) || Coordinate::m_distance[point][point_] == 1 || Coordinate::m_distance[point][point_] == 2){
        return true;
    }
    return false;

 //return true; 2024/4/15/ modify

}


//bool Board::legal_move(const coord_t &point, const coord_t &point_) const {
//    // 根据就判断，
//    return Coordinate::OnBoard(point) && Coordinate::OnBoard(point_) && grid[point_] == 0 && (grid[point] == -1 || grid[point] == 1) && ;
//
//}



coord_t Board::count_chess(const int16_t& player) const{
    coord_t count = 0;
    for(coord_t coord:Coordinates){
        if(grid[coord]==player){
            ++count;
        }
    }
    return count;
}



void Board::stone_down(const coord_t& coord,int16_t player) {
    if(Coordinate::OnBoard(coord) && Coordinate::Legal_coord(coord) && grid[coord] == NULL_COORD){
        grid[coord] = player;
    }
}

coord_t Board::stone_go(const coord_t& coord,const coord_t& coord_,int16_t player){
    if(Coordinate::OnBoard(coord) && Coordinate::Legal_coord(coord) && Coordinate::OnBoard(coord_) && Coordinate::Legal_coord(coord_)){
        if (is_jump(coord,coord_)) {
            // jump
            grid[coord] = NULL_COORD;
            grid[coord_] = player;
            //
//            if(Coordinate::m_distance[coord][coord_] == 2.0 || Coordinate::m_distance[coord][coord_] == 4.0 || Coordinate::m_distance[coord][coord_] ==
//                                                                                                                       sqrt(2)) {
            auto append = static_cast<int16_t>(abs(coord-coord_)/2);
            grid[std::max(coord, coord_)-append] = NULL_COORD;
//            }
            return std::max(coord,coord_)-append;
        }else{	
            grid[coord] = NULL_COORD;
            grid[coord_] = player;
            return -1;
        }
    }
}

bool Board::is_jump(const coord_t& coord,const coord_t& coord_) const{
    auto distance = Coordinate::m_distance[coord][coord_];
    if ((coord == 0 && coord_ == 18) || (coord == 18 && coord_ == 0) || (coord == 36 && coord_ == 18) || (coord == 18 && coord_ == 36) || (coord == 8 && coord_ == 26) || (coord == 26 && coord_ == 8) || (coord == 26 && coord_ == 44) || (coord == 44 && coord_ == 26)){
        return false;
    }else if (distance == 1 || distance == sqrt(2)){
        return false;
    }else if (distance == 2 * sqrt(2) || distance == 4.0 || distance == 2){
        return true;
    }else{
        return false;
    }
}





//bool Board::operation_square(const int16_t& player,const int16_t& squares) {
//    if(squares){
//        for(int16_t i=0;i<squares;++i){
//            coord_t coord = get_random_index(count_chess(-player));
//            grid[coord] = 0;
//            std::cout<<"square:point(row="<<std::to_string(std::get<1>(Coordinate::GetXY(coord)))<<",col="<<std::to_string(std::get<0>(Coordinate::GetXY(coord)))<<")"<<"\t";
//        }
//        std::cout<<std::endl;
//        return true;
//    }
//    return false;
//}
//
//bool Board::operation_square_simple(const int16_t& player,const int16_t& squares) {
//    if(squares){
//        for(int16_t i=0;i<squares;++i){
//            coord_t coord = get_random_index(count_chess(-player));
//            grid[coord] = 0;
////            std::cout<<"square:point(row="<<std::to_string(std::get<1>(Coordinate::GetXY(coord)))<<",col="<<std::to_string(std::get<0>(Coordinate::GetXY(coord)))<<")"<<"\t";
//        }
////        std::cout<<std::endl;
//        return true;
//    }
//    return false;
//}



//void Board::place_stone(const Move& move,int16_t player) {
//    if (Move::is_down(move)){
//        if(Coordinate::OnBoard())
//    }
//    if(Move::is_go(move)){
//
//    }



//    if(Coordinate::OnBoard(move.m_point)){
//
////        if (grid[move.m_point] == 0) {
//            grid[move.m_point] = player;
////        }
//
//    } else {
//        if(move.m_points.size()<3){
//            grid[move.m_points[0]] = 0;
//            grid[move.m_points[1]] = player;
//            if(Coordinate::m_distance[move.m_points[0]][move.m_points[1]]==2.0){
//                auto append = static_cast<int16_t>(abs(move.m_points[0]-move.m_points[1])/2);
//                grid[std::max(move.m_points[0], move.m_points[1])-append] = 0;
//            }
//        } else{
//            // 连
//            for(size_t i = 1;i<move.m_points.size();i++){
//                if(Coordinate::m_distance[move.m_points[i]][move.m_points[i-1]]==2.0){
//                    auto append = static_cast<int16_t>(abs(move.m_points[i]-move.m_points[i-1])/2);
//                    grid[std::max(move.m_points[i],move.m_points[i-1])-append] = 0;
//                }
//            }
//            grid[move.m_points[0]] = 0;
//            grid[move.m_points[move.m_points.size()-1]] = player;
//        }
//    }

//}



