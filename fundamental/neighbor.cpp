#include "neighbor.h"



template<> coord_t Neighbor<1, 1, false>::m_neighbor[SQUARE][MAX_NUM] = {0};
template<> coord_t Neighbor<2, 2, false>::m_neighbor[SQUARE][MAX_NUM] = {0};
template<> coord_t Neighbor<1, 4, false>::m_neighbor[SQUARE][MAX_NUM] = {0};
template<> coord_t Neighbor<1, 2, false>::m_neighbor[SQUARE][MAX_NUM] = {0};
template<> coord_t Neighbor<4, 4, false>::m_neighbor[SQUARE][MAX_NUM] = {0};
template<> coord_t Neighbor<1, 5, false>::m_neighbor[SQUARE][MAX_NUM] = {0};
template<> coord_t Neighbor<1, 10, false>::m_neighbor[SQUARE][MAX_NUM] = {0};
template<> coord_t Neighbor<1, 13, false>::m_neighbor[SQUARE][MAX_NUM] = {0};
template<> coord_t Neighbor<1, 20, false>::m_neighbor[SQUARE][MAX_NUM] = {0};

template<> coord_t Neighbor<1, 3, false>::m_neighbor[SQUARE][MAX_NUM] = {0};


template<> coord_t Neighbor<1, 1, true>::m_neighbor[SQUARE][MAX_NUM] = {0};
template<> coord_t Neighbor<2, 2, true>::m_neighbor[SQUARE][MAX_NUM] = {0};
template<> coord_t Neighbor<1, 4, true>::m_neighbor[SQUARE][MAX_NUM] = {0};
template<> coord_t Neighbor<1, 2, true>::m_neighbor[SQUARE][MAX_NUM] = {0};
template<> coord_t Neighbor<1, 5, true>::m_neighbor[SQUARE][MAX_NUM] = {0};
template<> coord_t Neighbor<4, 4, true>::m_neighbor[SQUARE][MAX_NUM] = {0};

template<> coord_t Neighbor<1, 3, true>::m_neighbor[SQUARE][MAX_NUM] = {0};

template<> int Neighbor<1, 1, false>::m_number[SQUARE] = {0};
template<> int Neighbor<2, 2, false>::m_number[SQUARE] = {0};
template<> int Neighbor<1, 4, false>::m_number[SQUARE] = {0};
template<> int Neighbor<1, 2, false>::m_number[SQUARE] = {0};
template<> int Neighbor<4, 4, false>::m_number[SQUARE] = {0};
template<> int Neighbor<1, 5, false>::m_number[SQUARE] = {0};
template<> int Neighbor<1, 10, false>::m_number[SQUARE] = {0};
template<> int Neighbor<1, 13, false>::m_number[SQUARE] = {0};
template<> int Neighbor<1, 20, false>::m_number[SQUARE] = {0};

template<> int Neighbor<1, 3, false>::m_number[SQUARE] = {0};

template<> int Neighbor<1, 1, true>::m_number[SQUARE] = {0};
template<> int Neighbor<2, 2, true>::m_number[SQUARE] = {0};
template<> int Neighbor<1, 4, true>::m_number[SQUARE] = {0};
template<> int Neighbor<1, 2, true>::m_number[SQUARE] = {0};
template<> int Neighbor<1, 5, true>::m_number[SQUARE] = {0};
template<> int Neighbor<4, 4, true>::m_number[SQUARE] = {0};

//template<> int Neighbor<1, 3, true>::m_number[SQUARE] = {0};

Zobrist zobrist;
Nakade nakade;

