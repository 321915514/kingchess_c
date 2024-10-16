//
// Created by wangxu on 2023/8/7.
//

#include "coordinate.h"

coord_t Coordinate::CalcSymmetry(coord_t coord, int16_t code) {
    int16_t x = coord % m_size;
    int16_t y = coord / m_size;
    if (coord >= 81) {
        return coord;
    }
    if (code >= 4) {
        y = m_size - 1 - y;
    }
    int16_t rotate = code % 4;
    if (rotate == 0) {
        return x + y * m_size;
    } else if (rotate == 1) {
        return y + (m_size - 1 - x) * m_size;
    } else if (rotate == 2) {
        return (m_size - 1 - x) + (m_size - 1 - y) * m_size;
    } else {
        return (m_size - 1 - y) + x * m_size;
    }
}

int16_t Coordinate::m_size = 0;
int16_t Coordinate::m_square = 0;
coord_t Coordinate::m_line[SQUARE];
//bool Coordinate::m_jump[SQUARE];
coord_t Coordinate::m_symmetry[81][8];
bool Coordinate::m_is_corner[SQUARE];
double Coordinate::m_distance[SQUARE][SQUARE];
double Coordinate::m_life_death_distance[SQUARE][SQUARE];
double Coordinate::m_influence[SQUARE][SQUARE];

CoordinateIterator Coordinates(0);
