//
// Created by wangxu on 2023/8/7.
//

#ifndef LONGCHESS_MCTS_CHESSTYPES_H
#define LONGCHESS_MCTS_CHESSTYPES_H

#include<vector>
#include <cstdint>
#include <valarray>
#include <tuple>
#include <cstring>
#define BOARDER -4
#define SIZE 9
#define SQUARE 45
#define NULL_COORD 0
#define BLACK 1
#define WHITE -1
#define DRAW 2
typedef int16_t coord_t;
typedef int16_t color_t;

class Relative {
public:
    Relative(){}

    int Area() const {
        if (m_delta_y > 0) {
            return 3;
        } else if (m_delta_y < 0) {
            return 1;
        } else if (m_delta_x > 0) {
            return 0;
        } else {
            return 2;
        }
    }

    int Distance() const {
        return m_delta_x * m_delta_x + m_delta_y * m_delta_y;
    }

    bool operator<(const Relative& relative) const {
        if (Area() < relative.Area()) {
            return true;
        } else if (Area() > relative.Area()) {
            return false;
        } else if (Area() == 0 || Area() == 2) {
            return (Distance() < relative.Distance());
        } else {
            int virtual_part = m_delta_y * relative.m_delta_x - m_delta_x * relative.m_delta_y;
            if (virtual_part > 0) {
                return true;
            } else if (virtual_part < 0) {
                return false;
            } else {
                return (Distance() < relative.Distance());
            }
        }
    }

public:
    int m_delta_x;
    int m_delta_y;
};


enum Status{
    Begin,
    Stage,
};

class Coordinate {
public:
    static int16_t m_size;
    static int16_t m_square;
    static coord_t m_line[SQUARE];
    static bool m_legal[SQUARE];
    static coord_t m_symmetry[81][8];
    static bool m_is_corner[SQUARE];
    static double m_distance[SQUARE][SQUARE];
    static double m_life_death_distance[SQUARE][SQUARE];
    static double m_influence[SQUARE][SQUARE];

    static const int16_t kMirror0Rotate0 = 0;
    static const int16_t kMirror0Rotate1 = 1;
    static const int16_t kMirror0Rotate2 = 2;
    static const int16_t kMirror0Rotate3 = 3;
    static const int16_t kMirror1Rotate0 = 4;
    static const int16_t kMirror1Rotate1 = 5;
    static const int16_t kMirror1Rotate2 = 6;
    static const int16_t kMirror1Rotate3 = 7;

    static coord_t Coord(int16_t x, int16_t y) { // y是行 x是列
        return x + y * m_size;
    }

    static std::tuple<int16_t, int16_t> GetXY(coord_t coord) {
        return std::make_tuple(coord % m_size, coord / m_size); //20  2，2 21  3,2  2,2---2,3
    }

    static int16_t GetX(coord_t coord) {
        return coord % m_size;
    }

    static int16_t GetY(coord_t coord) {
        return coord / m_size;
    }

    static bool OnBoard(int16_t x, int16_t y) {
        return x >= 0 && x < m_size && y >= 0 && y < m_size;
    }

    static bool Legal_coord(coord_t coord){
        if (coord == 1 || coord == 7 || coord == 9 || coord == 17 || coord == 27 || coord == 35 || coord == 37 || coord == 43){
            return false;
        }
        return true;
    }

    static bool OnBoard(coord_t coord) {
        return coord >= 0 && coord < m_square;
    }



    // 0-3 Rotate 0~270
    // 4-7 Reflect then Rotate 0~270
    static coord_t Symmetry(coord_t coord, int16_t code) {
        if (coord == NULL_COORD) {
            return coord;
        }
        return m_symmetry[coord][code];
    }

    static void GetRelative(coord_t base, coord_t another, Relative* relative) {
        relative->m_delta_x = GetX(another) - GetX(base);
        relative->m_delta_y = GetY(another) - GetY(base);
    }

    static coord_t AwayFromLine1(coord_t coord) {
        int16_t x = GetX(coord);
        int16_t y = GetY(coord);
        x = std::min(x, static_cast<int16_t>(m_size - 2));
        x = std::max(static_cast<int16_t>(1), x);

        y = std::min(y, static_cast<int16_t>(m_size - 2));
        y = std::max(static_cast<int16_t>(1), y);
        return Coord(x, y);
    }

    static coord_t CalcSymmetry(coord_t coord, int16_t code);

//    static void Reset_m_jump(){
//        memset(m_jump, false,sizeof(m_jump));
//    }

    static void SetBoardSize(int16_t row,int16_t size) {
        m_size = size;
        m_square = row * size;

        for(coord_t c = 0; c < 81; c++){
            for (int16_t code = 0; code < 8; ++code) {
                m_symmetry[c][code] = CalcSymmetry(c, code);
            }
        }


        for (coord_t coord = 0; coord < SQUARE; ++coord) {
            int16_t x = GetX(coord);
            int16_t y = GetY(coord);
            int16_t line_x = std::min(x + 1, m_size - x);
            int16_t line_y = std::min(y + 1, m_size - y);
            m_line[coord] = std::min(line_x, line_y);

//            m_jump[coord] = false;

            m_is_corner[coord] = ((x == 0 || x == size - 1) && (y == 0 || y == size - 1));
            for (coord_t another = 0; another < SQUARE; ++another) {
                Relative relative;
                GetRelative(coord, another, &relative);
                m_distance[coord][another] = sqrt(relative.Distance());
                m_influence[coord][another] = 1.0 * pow(0.1, m_distance[coord][another]);
            }
        }
        for (coord_t coord = 0; coord < SQUARE; ++coord) {
            for (coord_t another = 0; another < SQUARE; ++another) {
                m_life_death_distance[coord][another] =
                        m_distance[AwayFromLine1(coord)][AwayFromLine1(another)];
            }
        }
    }
};

class CoordinateIterator {
public:
    explicit CoordinateIterator(coord_t coord) : m_coord(coord) {}

    inline const CoordinateIterator begin() const {
        return CoordinateIterator(0);
    }

    inline const CoordinateIterator end() const {
        return CoordinateIterator(Coordinate::m_square);
    }

    inline void operator++() {
        ++m_coord;
    }

    inline coord_t operator*() {
        return m_coord;
    }

    inline bool operator!=(const CoordinateIterator& on_boards) {
        return m_coord != on_boards.m_coord;
    }

protected:
    coord_t m_coord;
};

extern CoordinateIterator Coordinates;







#endif //LONGCHESS_MCTS_CHESSTYPES_H
