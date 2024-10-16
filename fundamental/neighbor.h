#pragma once

#include <math.h>
#include "coordinate.h"
#include "zobrist.h"
#include <iostream>


template <int min, int max, bool include_boarder>
class Neighbor {
public:
    coord_t* begin() {
        return &(m_neighbor[m_base][0]);
    }
    coord_t* end() {
        return &(m_neighbor[m_base][m_number[m_base]]);
    }

    explicit Neighbor(coord_t base)
            : m_base(base) {}

    static void GetRelatives(std::vector<Relative>* relatives) {
        relatives->clear();

//        if(min == 1 && max == 3){
//            for (int delta_y = -2; delta_y <= 2; ++delta_y) {
//                for (int delta_x = -2; delta_x <= 2; ++delta_x) {
//                        Relative relative;
//                        relative.m_delta_x = delta_x;
//                        relative.m_delta_y = delta_y;
//                        if (relative.Distance() == 1 || relative.Distance() == sqrt(2) || relative.Distance() == 2*
//                                                                                                                 sqrt(2)) {
//                            relatives->push_back(relative);
//                        }
//                    }
//                }
//        }else {

            for (int delta_y = -4; delta_y <= 4; ++delta_y) {
                for (int delta_x = -4; delta_x <= 4; ++delta_x) {
                    Relative relative;
                    relative.m_delta_x = delta_x;
                    relative.m_delta_y = delta_y;
                    if (relative.Distance() >= min && relative.Distance() <= max) {
                        relatives->push_back(relative);
                    }
                }
            }
//        if(min == 1 && max == 4){
//            for(int i = -2; i <= 2; i+=4){
//                for(int j = -2; j <= 2; j+=4) {
//                    Relative relative;
//                    relative.m_delta_x = i;
//                    relative.m_delta_y = j;
//                    relatives->push_back(relative);
//                }
//            }
//        }
//        }




//        for(auto it:*relatives){
//            std::cout<<it.m_delta_x<<it.m_delta_y<<"\t"; // 0 -1,-1 0,1 0,0 1
//        }

//        std::sort(relatives->begin(), relatives->end());
//        std::rotate(relatives->begin(), relatives->end()-2, relatives->end());

        // -1 0,0 1,1 0,0 -1
    }

    static void Init() {
        if (min > max) {
            std::cerr << "Initialize Neighbor fatal error. min "
                      << min << " should less than max " << max;
        }
        std::vector<Relative> relatives;
        GetRelatives(&relatives);

        for (coord_t coord : Coordinates) {
            m_number[coord] = 0;
            for (int i = 0; i < relatives.size(); ++i) {

//                std::cout<<Coordinate::GetX(coord)<<std::endl;
//                std::cout<<relatives[i].m_delta_x<<std::endl;
                int16_t x = Coordinate::GetX(coord) + relatives[i].m_delta_x;
                int16_t y = Coordinate::GetY(coord) + relatives[i].m_delta_y;
                if (Coordinate::OnBoard(x, y)) {
                    m_neighbor[coord][m_number[coord]] = Coordinate::Coord(x, y);
                    ++(m_number[coord]);
                } else {
                    if (include_boarder) {
                        m_neighbor[coord][m_number[coord]] = BOARDER;
                        ++(m_number[coord]);
                    }
                }
            }
        }
    }

protected:
    coord_t m_base;
    static const int MAX_NUM = 68;
    static coord_t m_neighbor[SQUARE][MAX_NUM];
    static int m_number[SQUARE];
};

typedef Neighbor<1, 1, false> Neighbor4;
typedef Neighbor<2, 2, false> DiagNeighbor4;
typedef Neighbor<1, 2, false> Neighbor8;
typedef Neighbor<1, 4, false> Neighbor12;

//typedef Neighbor<1, 6, false> Neighbor16;

typedef Neighbor<4, 4, false> FarNeighbor4;
typedef Neighbor<1, 5, false> Neighbor20;
typedef Neighbor<1, 10, false> Neighbor36;
typedef Neighbor<1, 13, false> Neighbor44;
typedef Neighbor<1, 20, false> Neighbor68;

//
//  Neighbo4  DiagNeighbor4   Neighbor8   Neighbor12   FarNeighbor4    Neighbor16
//                                            O             O            0   0   0
//     O          O   O        O O O        O O O                          0 0 0
//   O   O                     O   O      O O   O O     O       O        0 0   0 0
//     O          O   O        O O O        O O O                          0 0 0
//                                            O             O            0   0   0
//
//   Neighbor20      Neighbor36         Neighbor44           Neighbor68
//                                                           O O O O O
//                     O O O            O O O O O          O O O O O O O
//     O O O         O O O O O        O O O O O O O      O O O O O O O O O
//   O O O O O     O O O O O O O      O O O O O O O      O O O O O O O O O
//   O O   O O     O O O   O O O      O O O   O O O      O O O O   O O O O
//   O O O O O     O O O O O O O      O O O O O O O      O O O O O O O O O
//     O O O         O O O O O        O O O O O O O      O O O O O O O O O
//                     O O O            O O O O O          O O O O O O O
//                                                           O O O O O
//
typedef Neighbor<1, 1, true> AllNeighbor4;
typedef Neighbor<2, 2, true> AllDiagNeighbor4;
typedef Neighbor<1, 2, true> AllNeighbor8;
typedef Neighbor<1, 4, true> AllNeighbor12;
typedef Neighbor<1, 5, true> AllNeighbor20;
typedef Neighbor<4, 4, true> AllFarNeighbor4;




