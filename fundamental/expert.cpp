//
// Created by 32191 on 2024/8/23.
//

#include "expert.h"

#include <bits/stl_pair.h>

#include "function.h"
#include "utils.h"

bool is_border(const coord_t coord) {
    return coord == 8 or coord == 18 or coord == 26 or coord == 36 or coord == 44 or coord == 0;
}

double generateRandomNumber() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(0.0, 1.0);
    return dis(gen);
}

bool legal_pos(const coord_t coord) {
    if (coord >= 0 && coord != 1 && coord != 7 && coord != 9 && coord != 17 && coord != 27 && coord != 35 && coord != 37 && coord != 43 && coord < 45) {
        return true;
    }
    return false;

}

coord_t get_three_point(coord_t point,coord_t neighbor) {
    auto distence = Coordinate::m_distance[point][neighbor];
    if(distence == 1.0) {
        if(neighbor+1 == point) {
            return point-2;
        }
        if(point+1 == neighbor) {
            return point+1;
        }
        if(point-neighbor == 9) {
            return point-18;
        }
        if(neighbor-point == 9) {
            return point+18;
        }
    }
    if(distence == sqrt(2)) {
        if(neighbor+10==point) {
            return point-20;
        }
        if(neighbor+8 == point) {
            return point-16;
        }
        if(point+10 == neighbor) {
            return point+20;
        }
        if(point+8 == neighbor) {
            return point+16;
        }
    }
}




std::unordered_map<Move,double,hashfunction,equalunction> Expert_agent::select_move(GameState &game) {
    // 白棋
    if(game.player == WHITE) {
        if(game.play_out<=32) {
            // 白棋落子
            std::unordered_map<coord_t,double,hashfunction,equalunction> score_moves;
            std::unordered_map<Move,double,hashfunction,equalunction> score_down_moves;
            std::vector<coord_t> candidates;
            std::vector<coord_t> kings;
            for(size_t i=0;i<SQUARE;i++) {
                if(!game.board.grid[i] && Coordinate::Legal_coord(i)) {
                    // 棋盘为空的位置
                    candidates.push_back(i);
                }
                if(game.board.grid[i] == BLACK) {
                    kings.push_back(i);
                }
            }
            for(auto black:kings) {
                std::vector<Move> point_s;
                game.one_king_legal_move(black,point_s);
                // 判空

                for(auto possble_point:candidates) {
                    if((Coordinate::m_distance[black][possble_point] == 1 || Coordinate::m_distance[black][possble_point] == sqrt(2)) && ! is_border(possble_point)) {
                        auto get_three = get_three_point(black, possble_point);
                        if(Coordinate::Legal_coord(get_three) && Coordinate::OnBoard(get_three)) {
                            if(game.board.grid[get_three] == 0) {
                                point_s.emplace_back(black, get_three); // 14
                            }
                        }
                    }
                }
                for(const auto& score_move:point_s) {
                    auto coord_ = score_move.m_point_;
                    auto count=0;
                    for(auto possible_point:kings) {
                        if((Coordinate::m_distance[coord_][possible_point] == 2 || Coordinate::m_distance[coord_][possible_point] ==2*sqrt(2)) && !is_border(possible_point) && game.board.grid[possible_point] == BLACK ) {
                            count++;
                        }
                    }
                    if(count==2) score_moves[coord_] = score_moves[coord_] +100 + generateRandomNumber();

                if(game.board.is_jump(black,coord_)) {
                    coord_t mid_coord = std::max(black,coord_) - std::abs(black-coord_)/2;
                    if(game.board.grid[mid_coord] == WHITE && game.board.check_move1(coord_,mid_coord) && game.board.check_move1(mid_coord,black)) {
                        score_moves[coord_] += 150 + generateRandomNumber();
                    }else if(game.board.grid[mid_coord] == NULL_COORD && game.board.check_move1(coord_,mid_coord) && game.board.check_move1(mid_coord,black)) {
                        score_moves[coord_] += 100+ generateRandomNumber();
                    }
                }else if((Coordinate::m_distance[black][coord_] == 2 || Coordinate::m_distance[black][coord_] == 2*sqrt(2)) && !is_border(black)) {
                    // std::cout<<"1"<<std::endl;
                    coord_t mid_coord = std::max(black,coord_) - abs(black-coord_)/2;
                    if(game.board.grid[mid_coord] == NULL_COORD && game.board.check_move1(coord_,mid_coord) && game.board.check_move1(mid_coord,black)) {
                        score_moves[coord_] += 100+ generateRandomNumber();
                    }
                }if(Coordinate::m_distance[black][coord_] == sqrt(2) || Coordinate::m_distance[black][coord_] == 1) {
                    if(coord_>black) {
                        auto max_coord = (std::max(coord_,black)) + abs(black-coord_);
                        if(legal_pos(max_coord) && game.board.grid[max_coord] == WHITE && game.board.check_move1(coord_,black) && game.board.check_move1(coord_,max_coord)) {
                            score_moves[coord_] = 100+ generateRandomNumber();
                        }else {
                            if(!game.board.check_move1(coord_,black) || !game.board.check_move1(coord_,max_coord)) {
                                score_moves[coord_] = 0+ generateRandomNumber();
                            }else {
                                score_moves[coord_] = -100+ generateRandomNumber();
                            }
                        }
                    }
                    if(coord_<black) {
                        auto min_coord = (std::min(coord_,black)) - abs(black-coord_);
                        if(legal_pos(min_coord) && game.board.grid[min_coord] == WHITE && game.board.check_move1(coord_,min_coord) && game.board.check_move1(coord_,black)) {
                            score_moves[coord_] = 100+ generateRandomNumber();
                        }else {
                            if(!game.board.check_move1(coord_,black) || !game.board.check_move1(coord_,min_coord)) {
                                score_moves[coord_] = 0+ generateRandomNumber();
                            }else {
                                score_moves[coord_] = -100+ generateRandomNumber();
                            }
                        }
                    }
                }if((black == 0 || black == 8) && coord_ == black+36) {
                    score_moves[coord_] = 70+ generateRandomNumber();
                }if((black == 44 || black == 36) && coord_ == black-36) {
                    score_moves[coord_] = 70+ generateRandomNumber();
                }if((black == 18 || black == 26) && coord_ == black-18) {
                    score_moves[coord_] = 70+ generateRandomNumber();
                }



                }

                if(black == 19) {
                    if(game.board.grid[black-1] == NULL_COORD) {
                        score_moves[black-1] = 70+ generateRandomNumber();
                    }
                    if(game.board.grid[black-9] == NULL_COORD) {
                        score_moves[black-9] = 70+ generateRandomNumber();
                    }
                    if(game.board.grid[black+9] == NULL_COORD) {
                        score_moves[black-9] = 70+ generateRandomNumber();
                    }
                }
                if(black == 25) {
                    if(game.board.grid[black+1] == NULL_COORD) {
                        score_moves[black+1] = 70+ generateRandomNumber();
                    }
                    if(game.board.grid[black-9] == NULL_COORD) {
                        score_moves[black-9] = 70+ generateRandomNumber();
                    }
                    if(game.board.grid[black+9] == NULL_COORD) {
                        score_moves[black-9] = 70+ generateRandomNumber();
                    }
                }
            }
            for(auto i:candidates) {
                if(!score_moves.count(i)) {
                    score_moves[i] =  generateRandomNumber();
                }
            }

            for(auto black:kings) {
                if(Coordinate::GetY(black)<2 && score_moves[20] >1) {
                    score_moves[20] +=100;
                }
                if(Coordinate::GetY(black)>6 && score_moves[24] >1) {
                    score_moves[24] +=100;
                }

            }

            for(auto coord:score_moves) {
                score_down_moves[Move(coord.first,-1)] = coord.second;
            }
            return score_down_moves;

        }else {
            // 白棋走棋
            std::unordered_map<Move,double,hashfunction,equalunction> white_score_routes;
            std::vector<Move> white_routes;
            game.legal_moves(white_routes);
            std::vector<coord_t> black_candidates;
            for(size_t i=0;i<SQUARE;i++) {
                if(game.board.grid[i] == BLACK) {
                    black_candidates.push_back(i);
                }
            }
            for(auto black:black_candidates) {
                std::vector<Move> point_s;
                game.one_king_legal_move(black,point_s);
                for(auto score_point:point_s) {
                    if(game.board.is_jump(black,score_point.m_point_)) {
                        for(auto white_move:white_routes) {
                            if(white_move.m_point_ == score_point.m_point_) {
                                white_score_routes[white_move] =100+ generateRandomNumber();
                            }
                            if((score_point.m_point_ == 0 || score_point.m_point_ == 8 || score_point.m_point_ == 18 || score_point.m_point_ == 26) && white_move.m_point_ == score_point.m_point_+18) {
                                white_score_routes[white_move] =100+ generateRandomNumber();
                            }
                            if((score_point.m_point_ == 18 || score_point.m_point_ == 26 || score_point.m_point_ == 36 || score_point.m_point_ == 44) && white_move.m_point_ == score_point.m_point_-18) {
                                white_score_routes[white_move] =100+ generateRandomNumber();
                            }

                        }
                    }

                    else {
                        if(Coordinate::m_distance[score_point.m_point][black] == 2 || Coordinate::m_distance[score_point.m_point][black] == 2*sqrt(2) and !is_border(black)) {
                            // * * X;
                            if(game.board.grid[score_point.m_point_] == NULL_COORD) {
                                for(auto white_move:white_routes) {
                                    if(white_move.m_point_ == score_point.m_point_ && game.board.grid[score_point.m_point_] == NULL_COORD) {
                                        white_score_routes[white_move] =100+ generateRandomNumber();
                                    }
                                }
                            }else if(game.board.grid[score_point.m_point_] == WHITE) {
                                for(auto white_move:white_routes) {
                                    auto mid_coord = (black+white_move.m_point_)/2;
                                    if(white_move.m_point_ == mid_coord && white_move.m_point!=score_point.m_point_ && game.board.grid[mid_coord] == NULL_COORD) {
                                        white_score_routes[white_move] =100+ generateRandomNumber();
                                    }
                                }
                            }
                        }

                        else if(is_border(black) && Coordinate::m_distance[score_point.m_point][black] == 2 && is_border(score_point.m_point)) {
                            if(black == 18 || black == 26) {
                                for(auto white_move:white_routes) {
                                    if(white_move.m_point_ == score_point.m_point_ && game.board.grid[score_point.m_point_] == NULL_COORD) {
                                        white_score_routes[white_move] =100+ generateRandomNumber();
                                    }
                                }
                            }
                            if((black == 0 || black == 8 ) && game.board.grid[black+36] == WHITE) {
                                for(auto white_move:white_routes) {
                                    if(white_move.m_point_ == score_point.m_point_ && game.board.grid[score_point.m_point_] == NULL_COORD) {
                                        white_score_routes[white_move] =100+ generateRandomNumber();
                                    }
                                }
                            }
                            if((black == 36 || black == 44 ) && game.board.grid[black-36] == WHITE) {
                                for(auto white_move:white_routes) {
                                    if(white_move.m_point_ == score_point.m_point_ && game.board.grid[score_point.m_point_] == NULL_COORD) {
                                        white_score_routes[white_move] =100+ generateRandomNumber();
                                    }
                                }
                            }



                        }

                        else if(Coordinate::m_distance[black][score_point.m_point_] == 1 || Coordinate::m_distance[black][score_point.m_point_] == sqrt(2)) {
                            auto pre_point_coord = (score_point.m_point_*2)-black;
                            for(auto white_move:white_routes) {
                                if(legal_pos(pre_point_coord)) {
                                    if(game.board.grid[pre_point_coord] == WHITE && white_move.m_point_ == score_point.m_point_ && game.board.grid[score_point.m_point_] == NULL_COORD && pre_point_coord != white_move.m_point) {
                                        // auto mid_coord = (score_point.m_point_+black)/2;
                                        // if(Coordinate::m_distance[score_point.m_point_][black] == 2 && game.board.grid[mid_coord] == WHITE) {
                                        //     white_score_routes[white_move] = 100;
                                        // }else {
                                            white_score_routes[white_move] = 100+ generateRandomNumber();
                                        // }
                                    }

                                    else if(game.board.grid[score_point.m_point_] == NULL_COORD && game.board.check_move1(white_move.m_point,pre_point_coord) && white_move.m_point_ == pre_point_coord) {
                                        if(game.board.grid[pre_point_coord] == NULL_COORD && white_move.m_point_ == pre_point_coord) {
                                            white_score_routes[white_move] = 100+ generateRandomNumber();
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

            }


            for(auto m:white_routes) {
                if(!white_score_routes.count(m)) {
                    white_score_routes[m] = generateRandomNumber();
                }
            }

            return white_score_routes;

        }
    }else {
        // 黑棋
        std::unordered_map<Move,double,hashfunction,equalunction> black_score_routes;
        std::vector<Move> black_routes;
        game.legal_moves(black_routes);
        if(black_routes.size() == 0) {
            // print_board(game.board);
            // std::cout<<"black is empty"<<std::endl;
            black_score_routes[Move(-100,-100)] = 0;
            return black_score_routes;
        }
        for(auto m:black_routes) {
            if(game.board.is_jump(m.m_point,m.m_point_)) {
                black_score_routes[m] = 100+ generateRandomNumber();
            }else {
                black_score_routes[m] = generateRandomNumber();
            }
        }
        return black_score_routes;
    }

}