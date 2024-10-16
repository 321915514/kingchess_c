//
// Created by wangxu on 2023/10/18.
//

#ifndef LONGCHESS_FUNCTION_H
#define LONGCHESS_FUNCTION_H

#include "board.h"

struct hashfunction {
    size_t operator()(const Move& move) const {
        if(Coordinate::OnBoard(move.m_point)){
            return std::hash<int>()(move.m_point);
        } else {
            return std::hash<int>()(move.m_point) ^ std::hash<int>()(move.m_point_);
        }
    }
};

struct equalunction {
    size_t operator()(const Move& m1, const Move& m2) const {
       return m1.m_point == m2.m_point && m1.m_point_ == m2.m_point_;
    }
};


#endif //LONGCHESS_FUNCTION_H
