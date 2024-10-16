//
// Created by wangxu on 2023/10/18.
//

#ifndef LONGCHESS_MOVE_H
#define LONGCHESS_MOVE_H
#include "coordinate.h"
#include <string>
class Move {
public:
    coord_t m_point;
    coord_t m_point_;
//    int16_t m_squares;
//    int16_t m_squares_front;

    ~Move(){
//        m_points.clear();
    };

    static std::string move2str(const Move &move){
        if(Move::is_down(move)){
            auto row = move.m_point/9;
            auto col = move.m_point%9;
            return std::to_string(row) + std::to_string(col);
        } else if(Move::is_go(move)){
            auto row = move.m_point/9;
            auto col = move.m_point%9;
            auto row_ = move.m_point_/9;
            auto col_ = move.m_point_%9;
            return std::to_string(row) + std::to_string(col) + std::to_string(row_) + std::to_string(col_);
        }
    }

    Move(){
        m_point = -1;
        m_point_ = -1;
//        m_squares = 0;
//        m_squares_front = 0;
    }

    Move(const coord_t& point):m_point(point),m_point_(-1){}

    Move(const coord_t& point, const coord_t& point_):m_point(point),m_point_(point_) {}

    static bool is_down(const Move &move){
        if (move.m_point >= 0 && move.m_point_ < 0){
            return true;
        }
        return false;
    }
    static bool is_go(const Move &move){
        if (move.m_point >= 0 && move.m_point_ >= 0){
            return true;
        }
        return false;
    }

    bool operator == (const Move& other) const{
        if(m_point == other.m_point && m_point_ == other.m_point_){
            return true;
        }
        return false;

    }


   

//    Move(const std::vector<coord_t>& points,int16_t squares_front,int16_t squares) {
//        for(auto i:points){
//            m_points.push_back(i);
//        }
//        m_squares_front = squares_front;
//        m_squares = squares;
//    }

//    Move(coord_t point, const std::vector<coord_t>& points,int16_t squares_front,int16_t squares) {
//        m_point = point;
//        for(auto i:points){
//            m_points.push_back(i);
//        }
//        m_squares_front = squares_front;
//        m_squares = squares;
//    }


//    static Move PlayDown(const coord_t& point) {
//        return Move(point);
//    }
//    static Move playGo(cons coord_t& point, const coord_t& point_) {
//        return Move(point, point_);
//    }
//    static Move playStage2(const std::vector<coord_t>& points,int16_t squares) {
//        return Move(points,squares);
//    }
//    static Move playStage2(const std::vector<coord_t>& points,int16_t squares_front,int16_t squares) {
//        return Move(points,squares_front,squares);
//    }

    Move(const Move &other_move) :m_point(other_move.m_point),m_point_(other_move.m_point_){}

    Move& operator=(const Move &other_m) {
        if (&other_m == this) {
            return *this;
        }
        m_point = other_m.m_point;
        m_point_ = other_m.m_point_;
        return *this;

    }
};


#endif //LONGCHESS_MOVE_H
