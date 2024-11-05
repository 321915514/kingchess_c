//
// Created by wangxu on 2023/10/18.
//

#include "gamestate.h"
#include "coordinate.h"
#include "utils.h"
#include <unordered_map>
#include <deque>
#include <algorithm>
#include <list>
#include <float.h>

//void GameState::get_random_move_frist(coord_t &coord,std::unordered_map<coord_t, std::vector<std::vector<coord_t>>>& v_moves) const {
////    std::vector<coord_t> v_target_moves;
//    get_player_moves(v_moves);
//
////                std::unordered_map<coord_t,std::vector<coord_t>>::iterator it = v_moves.begin();
////
//
//    std::vector<coord_t> v_moves_first;
////    std::vector<coord_t> v_jump; // 跳了一下的坐标
//    for (auto it:v_moves) {
//        v_moves_first.push_back(it.first);
//    }
//    coord = v_moves_first[get_random_index(v_moves_first.size())];
//}


//void GameState::apply_move(const Move& move,GameState& newgame) const {
//    Board next_board = board;
//    if (is_valid_move_stage1(move)) {
//        next_board.place_stone(move, player);
//        newgame.board = next_board;
//        newgame.player = -player;
//        newgame.move = move;
//        newgame.jump_dict = {};
//    } else {
//        Board next_board = board;
//        if(move.m_point<0 && move.m_points.size()>0){
//            next_board.place_stone(move, player);
//            newgame.board = next_board;
//            newgame.player = -player;
//            newgame.move = move;
//            newgame.jump_dict = {};
//        }
//    }
//}


void GameState::apply_move(const Move &move, GameState &newgame) const {
    /**
     * king chess
     */
    Board next_board = board;
    if (Move::is_down(move)) {
        next_board.stone_down(move.m_point, player);
	//std::cout<<"moves:"<<newgame.moves.size()<<std::endl;
        newgame.eat_point = -1;
	newgame.moves.push_back(move);
    } else if (Move::is_go(move)) {
        coord_t stone = next_board.stone_go(move.m_point, move.m_point_, player);
	//std::cout<<"moves:"<<newgame.moves.size()<<std::endl;
	newgame.moves.push_back(move);
        if (stone != -1) {
            newgame.eat_point = stone;
        } else {
            newgame.eat_point = -1;
        }
    }
    newgame.board = next_board;
    newgame.player = -player;
    newgame.play_out++;
    newgame.move = move;
    newgame.moves = moves;
    //std::cout<<newgame.moves.size()<<std::endl;

}

coord_t GameState::eat_chess() const {
    /*
     * 黑吃白计数
     */
    int16_t board_chess = board.count_chess(WHITE);
    int16_t end_chess = play_out > 32 ? 0 : 16 - (play_out / 2);

    return 24 - board_chess - end_chess;
}

bool GameState::is_valid_move(const Move &move) const {
    if (Move::is_down(move)) {
        if (board.grid[move.m_point] == 0) {
            return true;
        }
    }
    if (Move::is_go(move)) {
        if (board.grid[move.m_point] != 0 && board.grid[move.m_point_] == 0 && board.check_move1(
                move.m_point, move.m_point_)) {
            return true;
        }
    }
    return false;
}


void GameState::get_legal_moves(const std::vector<coord_t> &coords, std::vector<Move> &moves) {
    //    std::cout<<coords[0]<<"-------"<<coords[1]<<std::endl;
    //    std::cout<<std::sqrt(2)<<std::endl;
    if (player == BLACK) {
        for (auto coord: coords) {
            //            std::vector<coord_t> target_move1_coords;


            //            std::rotate(AllNeighbor8(coord).begin(),AllNeighbor8(coord).begin()+6,AllNeighbor8(coord).end());

            //            for(auto it:AllNeighbor8(coord)){
            //                std::cout<< it<<"\t";
            //            }
            //            std::cout<<std::endl;


            if (coord == 0 && board.grid[coord + 18] == 0 && Coordinate::m_distance[coord][coord + 18] == 2) {
                moves.push_back(Move(coord, coord + 18));
            }
            if (coord == 18 && board.grid[coord + 18] == 0 && Coordinate::m_distance[coord][coord + 18] == 2) {
                moves.push_back(Move(coord, coord + 18));
            }
            if (coord == 8 && board.grid[coord + 18] == 0 && Coordinate::m_distance[coord][coord + 18] == 2) {
                moves.push_back(Move(coord, coord + 18));
            }
            if (coord == 26 && board.grid[coord + 18] == 0 && Coordinate::m_distance[coord][coord + 18] == 2) {
                moves.push_back(Move(coord, coord + 18));
            }
            if (coord == 26 && board.grid[coord - 18] == 0 && Coordinate::m_distance[coord][coord - 18] == 2) {
                moves.push_back(Move(coord, coord - 18));
            }
            if (coord == 44 && board.grid[coord - 18] == 0 && Coordinate::m_distance[coord][coord - 18] == 2) {
                moves.push_back(Move(coord, coord - 18));
            }


            for (auto coord_: AllNeighbor8(coord)) {
                // 21	12	11	10	19	28	29	30
                //                std::cout<<coord<<"---"<<coord_<<"  "<<Coordinate::m_distance[coord][coord_]<<std::endl;


                if (board.check_move1(coord, coord_)) {
                    // 可以走
                    if (board.grid[coord_] == 0 && Coordinate::m_distance[coord][coord_] < 2) {
                        //                        target_move1_coords.push_back(coord_);
                        moves.push_back(Move(coord, coord_));
                    } else if (coord == 18 && board.grid[coord - 18] == 0 && Coordinate::m_distance[coord][coord - 18]
                               == 2) {
                        moves.push_back(Move(coord, coord - 18));
                    } else if (coord == 36 && board.grid[coord - 18] == 0 && Coordinate::m_distance[coord][coord - 18]
                               == 2) {
                        moves.push_back(Move(coord, coord - 18));
                    } else if (coord == 0 && board.grid[coord + 18] == WHITE && board.grid[coord + 36] == 0 &&
                               Coordinate::m_distance[coord][coord + 36] == 4 && Coordinate::m_distance[coord][
                                   coord + 18] == 2) {
                        moves.push_back(Move(coord, coord + 36));
                    } else if (coord == 8 && board.grid[coord + 18] == WHITE && board.grid[coord + 36] == 0 &&
                               Coordinate::m_distance[coord][coord + 36] == 4 && Coordinate::m_distance[coord][
                                   coord + 18] == 2) {
                        moves.push_back(Move(coord, coord + 36));
                    } else if (board.grid[coord_] == WHITE) {
                        // 检查跳走
                        if (Coordinate::m_distance[coord][coord_] == 1) {
                            //  == 1

                            if (coord_ - coord == 1) {
                                // 右边
                                if (board.check_move1(coord_, coord_ + 1) && board.grid[coord] == BLACK &&
                                    board.grid[coord_] == WHITE && board.grid[coord_ + 1] == NULL_COORD) {
                                    moves.push_back(Move(coord, coord_ + 1));
                                }
                            } else if (coord - coord_ == 1) {
                                // 左边
                                if (board.check_move1(coord_, coord_ - 1) && board.grid[coord] == BLACK &&
                                    board.grid[coord_] == WHITE && board.grid[coord_ - 1] == NULL_COORD) {
                                    moves.push_back(Move(coord, coord_ - 1));
                                }
                            } else if (coord_ - coord == 9) {
                                // 下边
                                if (board.check_move1(coord_, coord_ + 9) && board.grid[coord] == BLACK &&
                                    board.grid[coord_] == WHITE && board.grid[coord_ + 9] == NULL_COORD) {
                                    moves.push_back(Move(coord, coord_ + 9));
                                }
                            } else if (coord - coord_ == 9) {
                                // 上边
                                if (board.check_move1(coord_, coord_ - 9) && board.grid[coord] == BLACK &&
                                    board.grid[coord_] == WHITE && board.grid[coord_ - 9] == NULL_COORD) {
                                    moves.push_back(Move(coord, coord_ - 9));
                                }
                            }
                        } else if (Coordinate::m_distance[coord][coord_] == std::sqrt(2)) {
                            // sqrt(2)

                            if (coord - coord_ == 8) {
                                //右上
                                if (board.check_move1(coord_, coord_ - 8) && board.grid[coord] == BLACK &&
                                    board.grid[coord_] == WHITE && board.grid[coord_ - 8] == NULL_COORD) {
                                    moves.push_back(Move(coord, coord_ - 8));
                                }
                            } else if (coord - coord_ == 10) {
                                // 左上
                                if (board.check_move1(coord_, coord_ - 10) && board.grid[coord] == BLACK &&
                                    board.grid[coord_] == WHITE && board.grid[coord_ - 10] == NULL_COORD) {
                                    moves.push_back(Move(coord, coord_ - 10));
                                }
                            } else if (coord_ - coord == 8) {
                                // 左下
                                if (board.check_move1(coord_, coord_ + 8) && board.grid[coord] == BLACK &&
                                    board.grid[coord_] == WHITE && board.grid[coord_ + 8] == NULL_COORD) {
                                    moves.push_back(Move(coord, coord_ + 8));
                                }
                            } else if (coord_ - coord == 10) {
                                // 右下
                                if (board.check_move1(coord_, coord_ + 10) && board.grid[coord] == BLACK &&
                                    board.grid[coord_] == WHITE && board.grid[coord_ + 10] == NULL_COORD) {
                                    moves.push_back(Move(coord, coord_ + 10));
                                }
                            }
                        }
                    }
                }
            }


            if (coord == 36 && board.grid[coord - 18] == WHITE && board.grid[coord - 36] == 0 && Coordinate::m_distance[
                    coord][coord - 36] == 4 && Coordinate::m_distance[coord][coord - 18] == 2) {
                moves.push_back(Move(coord, coord - 36));
            }
            if (coord == 44 && board.grid[coord - 18] == WHITE && board.grid[coord - 36] == 0 && Coordinate::m_distance[
                    coord][coord - 36] == 4 && Coordinate::m_distance[coord][coord - 18] == 2) {
                moves.push_back(Move(coord, coord - 36));
            }
        }


        // 检查黑棋的距离
        // 定义不合法元素的条件
        auto is_invalid = [](Move m) {
            return !((std::abs(Coordinate::m_distance[m.m_point][m.m_point_] - 1.0) < FLT_EPSILON) || (std::abs(
                         Coordinate::m_distance[m.m_point][m.m_point_] -
                         std::sqrt(2)) < FLT_EPSILON) || (
                         std::abs(Coordinate::m_distance[m.m_point][m.m_point_] - 2.0) < FLT_EPSILON) || (std::abs(
                         Coordinate::m_distance[m.m_point][m.m_point_] - 2 *
                         std::sqrt(2)) < FLT_EPSILON) || (
                         std::abs(Coordinate::m_distance[m.m_point][m.m_point_] - 4.0) < FLT_EPSILON));
        };


        //	std::cout<<moves.size()<<std::endl;

        // 删除不合法元素
        moves.erase(std::remove_if(moves.begin(), moves.end(), is_invalid), moves.end());

        //	std::cout<<moves.size()<<std::endl;
    } else if (player == WHITE) {
        if (play_out <= 32) {
            for (auto coord: Coordinates) {
                if (Coordinate::Legal_coord(coord) && !Coordinate::OnBoard(coord) && board.grid[coord] == 0) {
                    moves.push_back(coord);
                }
            }
        } else {
            for (auto coord: coords) {
                // add boarder move modify 24/4/19
                //

                if (coord == 0 && board.grid[coord + 18] == 0 && Coordinate::m_distance[coord][coord + 18] == 2) {
                    moves.push_back(Move(coord, coord + 18));
                }
                if (coord == 18 && board.grid[coord + 18] == 0 && Coordinate::m_distance[coord][coord + 18] == 2) {
                    moves.push_back(Move(coord, coord + 18));
                }
                if (coord == 8 && board.grid[coord + 18] == 0 && Coordinate::m_distance[coord][coord + 18] == 2) {
                    moves.push_back(Move(coord, coord + 18));
                }
                if (coord == 26 && board.grid[coord + 18] == 0 && Coordinate::m_distance[coord][coord + 18] == 2) {
                    moves.push_back(Move(coord, coord + 18));
                }
                if (coord == 26 && board.grid[coord - 18] == 0 && Coordinate::m_distance[coord][coord - 18] == 2) {
                    moves.push_back(Move(coord, coord - 18));
                }
                if (coord == 44 && board.grid[coord - 18] == 0 && Coordinate::m_distance[coord][coord - 18] == 2) {
                    moves.push_back(Move(coord, coord - 18));
                }


                for (auto coord_: AllNeighbor8(coord)) {
                    // 21	12	11	10	19	28	29	30
                    //                    std::cout << coord << "---" << coord_ << "  " << Coordinate::m_distance[coord][coord_] << std::endl;
                    if (board.check_move1(coord, coord_) && board.grid[coord_] == NULL_COORD) {
                        // 可以走
                        moves.push_back(Move(coord, coord_));
                    } else if (coord == 18 && board.grid[coord - 18] == 0 && Coordinate::m_distance[coord][coord - 18]
                               == 2) {
                        moves.push_back(Move(coord, coord - 18));
                    } else if (coord == 36 && board.grid[coord - 18] == 0 && Coordinate::m_distance[coord][coord - 18]
                               == 2) {
                        moves.push_back(Move(coord, coord - 18));
                    }
                }
            }
            // end
            // 定义不合法元素的条件
            auto is_invalid = [](Move m) {
                return !((std::abs(Coordinate::m_distance[m.m_point][m.m_point_] - 1.0) < FLT_EPSILON) || (std::abs(
                             Coordinate::m_distance[m.m_point][m.m_point_] -
                             std::sqrt(2)) < FLT_EPSILON) || (
                             std::abs(Coordinate::m_distance[m.m_point][m.m_point_] - 2.0) < FLT_EPSILON));
            };

            // 删除不合法元素
            moves.erase(std::remove_if(moves.begin(), moves.end(), is_invalid), moves.end());
        }
    }
}


void GameState::legal_moves(std::vector<Move> &moves) {

    //std::cout<<"legal moves"<<std::endl;
    if (player == BLACK) {
        // is black
        std::vector<coord_t> black_chess;
        for (coord_t coord: Coordinates) {
            if (board.grid[coord] == BLACK) {
                black_chess.push_back(coord);
            }
        }
        //
//	std::cout<<"get legal moves"<<std::endl;
        get_legal_moves(black_chess, moves);
//	std::cout<<"return moves black"<<std::endl;
        return;
    } else if (player == WHITE) {
        // is white
        if (play_out <= 32) {
//	    std::cout<<"white down"<<std::endl;
            for (auto coord: Coordinates) {
                if (Coordinate::Legal_coord(coord) && Coordinate::OnBoard(coord) && board.grid[coord] == 0) {
                    moves.emplace_back(coord);
                }
            }
	    
//	std::cout<<"return moves white down"<<std::endl;
            return;
        } else {
            std::vector<coord_t> white_chess;
            for (coord_t coord: Coordinates) {
                if (board.grid[coord] == WHITE) {
                    white_chess.push_back(coord);
                }
            }

//	std::cout<<"get legal moves white"<<std::endl;
        get_legal_moves(white_chess, moves);
	    
//	std::cout<<"return moves white go"<<std::endl;
            return;
        }
    }
}


void GameState::one_king_legal_move(const coord_t &coord, std::vector<Move> &moves) {
    if (coord == 0 && board.grid[coord + 18] == 0 && Coordinate::m_distance[coord][coord + 18] == 2) {
        moves.emplace_back(coord, coord + 18);
    }
    if (coord == 18 && board.grid[coord + 18] == 0 && Coordinate::m_distance[coord][coord + 18] == 2) {
        moves.emplace_back(coord, coord + 18);
    }
    if (coord == 8 && board.grid[coord + 18] == 0 && Coordinate::m_distance[coord][coord + 18] == 2) {
        moves.emplace_back(coord, coord + 18);
    }
    if (coord == 26 && board.grid[coord + 18] == 0 && Coordinate::m_distance[coord][coord + 18] == 2) {
        moves.emplace_back(coord, coord + 18);
    }
    if (coord == 26 && board.grid[coord - 18] == 0 && Coordinate::m_distance[coord][coord - 18] == 2) {
        moves.emplace_back(coord, coord - 18);
    }
    if (coord == 44 && board.grid[coord - 18] == 0 && Coordinate::m_distance[coord][coord - 18] == 2) {
        moves.emplace_back(coord, coord - 18);
    }


    for (auto coord_: AllNeighbor8(coord)) {
        // 21	12	11	10	19	28	29	30
        //                std::cout<<coord<<"---"<<coord_<<"  "<<Coordinate::m_distance[coord][coord_]<<std::endl;


        if (board.check_move1(coord, coord_)) {
            // 可以走
            if (board.grid[coord_] == 0 && Coordinate::m_distance[coord][coord_] < 2) {
                //                        target_move1_coords.push_back(coord_);
                moves.emplace_back(coord, coord_);
            } else if (coord == 18 && board.grid[coord - 18] == 0 && Coordinate::m_distance[coord][coord - 18] == 2) {
                moves.emplace_back(coord, coord - 18);
            } else if (coord == 36 && board.grid[coord - 18] == 0 && Coordinate::m_distance[coord][coord - 18] == 2) {
                moves.emplace_back(coord, coord - 18);
            } else if (coord == 0 && board.grid[coord + 18] == WHITE && board.grid[coord + 36] == 0 &&
                       Coordinate::m_distance[coord][coord + 36] == 4 && Coordinate::m_distance[coord][coord + 18] ==
                       2) {
                moves.emplace_back(coord, coord + 36);
            } else if (coord == 8 && board.grid[coord + 18] == WHITE && board.grid[coord + 36] == 0 &&
                       Coordinate::m_distance[coord][coord + 36] == 4 && Coordinate::m_distance[coord][coord + 18] ==
                       2) {
                moves.emplace_back(coord, coord + 36);
            } else if (board.grid[coord_] == WHITE) {
                // 检查跳走
                if (Coordinate::m_distance[coord][coord_] == 1) {
                    //  == 1

                    if (coord_ - coord == 1) {
                        // 右边
                        if (board.check_move1(coord_, coord_ + 1) && board.grid[coord] == BLACK &&
                            board.grid[coord_] == WHITE && board.grid[coord_ + 1] == NULL_COORD) {
                            moves.emplace_back(coord, coord_ + 1);
                        }
                    } else if (coord - coord_ == 1) {
                        // 左边
                        if (board.check_move1(coord_, coord_ - 1) && board.grid[coord] == BLACK &&
                            board.grid[coord_] == WHITE && board.grid[coord_ - 1] == NULL_COORD) {
                            moves.emplace_back(coord, coord_ - 1);
                        }
                    } else if (coord_ - coord == 9) {
                        // 下边
                        if (board.check_move1(coord_, coord_ + 9) && board.grid[coord] == BLACK &&
                            board.grid[coord_] == WHITE && board.grid[coord_ + 9] == NULL_COORD) {
                            moves.emplace_back(coord, coord_ + 9);
                        }
                    } else if (coord - coord_ == 9) {
                        // 上边
                        if (board.check_move1(coord_, coord_ - 9) && board.grid[coord] == BLACK &&
                            board.grid[coord_] == WHITE && board.grid[coord_ - 9] == NULL_COORD) {
                            moves.emplace_back(coord, coord_ - 9);
                        }
                    }
                } else if (Coordinate::m_distance[coord][coord_] == std::sqrt(2)) {
                    // sqrt(2)

                    if (coord - coord_ == 8) {
                        //右上
                        if (board.check_move1(coord_, coord_ - 8) && board.grid[coord] == BLACK &&
                            board.grid[coord_] == WHITE && board.grid[coord_ - 8] == NULL_COORD) {
                            moves.emplace_back(coord, coord_ - 8);
                        }
                    } else if (coord - coord_ == 10) {
                        // 左上
                        if (board.check_move1(coord_, coord_ - 10) && board.grid[coord] == BLACK &&
                            board.grid[coord_] == WHITE && board.grid[coord_ - 10] == NULL_COORD) {
                            moves.emplace_back(coord, coord_ - 10);
                        }
                    } else if (coord_ - coord == 8) {
                        // 左下
                        if (board.check_move1(coord_, coord_ + 8) && board.grid[coord] == BLACK &&
                            board.grid[coord_] == WHITE && board.grid[coord_ + 8] == NULL_COORD) {
                            moves.emplace_back(coord, coord_ + 8);
                        }
                    } else if (coord_ - coord == 10) {
                        // 右下
                        if (board.check_move1(coord_, coord_ + 10) && board.grid[coord] == BLACK &&
                            board.grid[coord_] == WHITE && board.grid[coord_ + 10] == NULL_COORD) {
                            moves.emplace_back(coord, coord_ + 10);
                        }
                    }
                }
            }
        }
    }


    if (coord == 36 && board.grid[coord - 18] == WHITE && board.grid[coord - 36] == 0 && Coordinate::m_distance[coord][
            coord - 36] == 4 && Coordinate::m_distance[coord][coord - 18] == 2) {
        moves.emplace_back(coord, coord - 36);
    }
    if (coord == 44 && board.grid[coord - 18] == WHITE && board.grid[coord - 36] == 0 && Coordinate::m_distance[coord][
            coord - 36] == 4 && Coordinate::m_distance[coord][coord - 18] == 2) {
        moves.emplace_back(coord, coord - 36);
    }
}


void GameState::is_gameover(int &winner) {
    //    print_move(move);
    //    std::cout<<eat_chess()<<std::endl;
    std::vector<Move> m;
    legal_moves(m);
    if(m.size() ==0) {
        winner = WHITE;
        return;
    }
    auto move_len = moves.size();
    if(move_len > 32 && move_len % 2 == 0 && moves.back() == moves[move_len-5]){
        winner = BLACK;
	return;
    }
    if (move.m_point == -100 && move.m_point_ == -100) {
        winner = WHITE;
	return;
    } else if (eat_chess() >= 11) {
        winner = BLACK;
	return;
    } else if (play_out > 300) {
        // legal_moves(moves);
	//if (moves.size() == 1) {
        //    winner = WHITE;
        //} else {
         winner = DRAW;
	 return;
        //}
    } else {
        winner = 0;
    }
}

int GameState::move_2_action(const Move &move) {
    if (player == BLACK) {
        if (Move::is_go(move)) {
            return move.m_point * 16 + transfer_pos(move);
        } else if (move.m_point == -100 && move.m_point_ == -100) {
            return -100;
        }
    }

    if (player == WHITE) {
        if (Move::is_down(move)) {
            auto a = 720 + move.m_point;
            return a;
        } else if (Move::is_go(move)) {
            auto a = 765 + move.m_point * 8 + transfer_pos(move);
            return a;
        }
    }
}


Move GameState::action_2_move(const int action) {
    // action to move

    if (action == -100) {
        return Move(-100, -100);
    }
    if (action >= 0 && action < 720) {
        return Move(action / 16, transfer_coord(action));
    }
    if (action >= 765) {
        return Move((action - 765) / 8, transfer_coord(action));
    }
    if (720 <= action && action < 765) {
        return Move((action - 720), -1);
    }
}

int GameState::transfer_pos(const Move &move) {
    if (Move::is_go(move)) {
        auto coord = move.m_point;
        auto coord_ = move.m_point_;
        if (coord == 0 and coord_ == 18) return 0;
        if (coord == 18 and coord_ == 0) return 4;
        if (coord == 0 and coord_ == 36) return 8;
        if (coord == 36 and coord_ == 0) return 12;

        else if (coord == 8 and coord_ == 26) return 0;
        else if (coord == 8 and coord_ == 44) return 8;
        else if (coord == 26 and coord_ == 8) return 4;
        else if (coord == 44 and coord_ == 8) return 12;

        else if (coord == 18 and coord_ == 36) return 0;
        else if (coord == 36 and coord_ == 18) return 4;
        else if (coord == 26 and coord_ == 44) return 0;
        else if (coord == 44 and coord_ == 26) return 4;

        else if (coord - coord_ == 1) // 左
            return 6;
        else if (coord - coord_ == -1) // 右
            return 2;
        else if (coord - coord_ == -9) // 下
            return 0;
        else if (coord - coord_ == 9) // 上
            return 4;
        else if (coord - coord_ == -8) // 左下
            return 7;
        else if (coord - coord_ == -10) // 右下
            return 1;
        else if (coord - coord_ == 8) // 右上
            return 3;
        else if (coord - coord_ == 10) // 左上
            return 5;
        else if (coord - coord_ == 2) // 左左
            return 14;
        else if (coord - coord_ == -2) // 右
            return 10;
        else if (coord - coord_ == -18) // 下下
            return 8;
        else if (coord - coord_ == 18) // 上上
            return 12;
        else if (coord - coord_ == -16) // 左下
            return 15;
        else if (coord - coord_ == -20) // 右下
            return 9;
        else if (coord - coord_ == 16) // 右上
            return 11;
        else if (coord - coord_ == 20) // 左上
            return 13;
    }
}


int GameState::transfer_coord(int action) {
    int coord, action_;

    if (action >= 765) {
        coord = (action - 765) / 8;
        action_ = (action - 765) % 8;
    } else {
        coord = action / 16;
        action_ = action % 16;
    }

    if ((coord == 0 || coord == 8) && action_ == 0) return coord + 18;
    if ((coord == 0 || coord == 8) && action_ == 8) return coord + 36;

    if ((coord == 36 || coord == 44) && action_ == 4) return coord - 18;
    if ((coord == 36 || coord == 44) && action_ == 12) return coord - 36;

    if ((coord == 18 || coord == 26) && action_ == 4) return coord - 18;
    if ((coord == 18 || coord == 26) && action_ == 0) return coord + 18;

    switch (action_) {
        case 0: return coord + 9;
        case 1: return coord + 10;
        case 2: return coord + 1;
        case 3: return coord - 8;
        case 4: return coord - 9;
        case 5: return coord - 10;
        case 6: return coord - 1;
        case 7: return coord + 8;
        case 8: return coord + 18;
        case 9: return coord + 20;
        case 10: return coord + 2;
        case 11: return coord - 16;
        case 12: return coord - 18;
        case 13: return coord - 20;
        case 14: return coord - 2;
        case 15: return coord + 16;
        default: throw std::invalid_argument("Invalid action value");
    }
}


//bool GameState::is_gameover() const{
//    if(board.count_chess(BLACK)==0 || board.count_chess(WHITE)==0){
//        return true;
//    } else{
//        return false;
//    }
//}
//
//bool GameState::is_valid_move_stage1(const Move& move)  const {
//    if(move.m_point>=0) {
//        auto it = board.grid[move.m_point];
//        return it == 0;
//    } else{
//        return false;
//    }
//}
//
//bool GameState::is_valid_move_stage3(const coord_t& coord) const {
//    if(board.grid[coord]==NULL_COORD){
//        return true;
//    } else{
//        return false;
//    }
//}
//
//
//bool GameState::is_square(const coord_t& coord,coord_t& square) const{
//
//    coord_t player = board.grid[coord];
//
//    std::vector<coord_t> neighbors;
//    neighbors.assign(Neighbor8(coord).begin(),Neighbor8(coord).end());
//
//
//    if(Coordinate::m_line[coord]>=2){
//
//        neighbors.push_back(neighbors[0]);
//
//        for(size_t i =2; i<neighbors.size();i+=2){
//            if(board.grid[neighbors[i-2]]==player && board.grid[neighbors[i-1]]==player && board.grid[neighbors[i]]==player){
//                ++square;
//            }
//        }
//    } else{
//        if(Coordinate::m_is_corner[coord]){
//            if(board.grid[neighbors[0]]==player && board.grid[neighbors[2]]==player && board.grid[neighbors[1]]==player){
//                ++square;
//            }
//        } else{
//            if(coord<Coordinate::m_size){
//                // bootom
//                neighbors.push_back(neighbors[0]);
//
//                if(board.grid[neighbors[1]]==player && board.grid[neighbors[2]]==player && board.grid[neighbors[3]]==player){
//                    ++square;
//                }
//                if(board.grid[neighbors[3]]==player && board.grid[neighbors[4]]==player && board.grid[neighbors[5]]==player) {
//                    ++square;
//                }
//            } else {
//                // right // top // left
//                if(board.grid[neighbors[1]]==player && board.grid[neighbors[2]]==player && board.grid[neighbors[0]]==player){
//                    ++square;
//                }
//                if(board.grid[neighbors[3]]==player && board.grid[neighbors[2]]==player && board.grid[neighbors[4]]==player) {
//                    ++square;
//                }
//            }
//        }
//    }
//    if(square){
//        return true;
//    }else{
//        return false;
//    }
//}
//
//
//bool GameState::is_move(const coord_t& coord,std::vector<coord_t>& v_jump_single_target_moves) const{
//    std::vector<coord_t> neighbor4(AllNeighbor4(coord).begin(), AllNeighbor4(coord).end());
//    std::vector<coord_t> farneighbor4(AllFarNeighbor4(coord).begin(), AllFarNeighbor4(coord).end());
//
//
//    bool flag = false;
//    for(size_t i =0; i<4;++i){
//        if(Coordinate::OnBoard(neighbor4[i]) && Coordinate::OnBoard(farneighbor4[i])) {
//            if (board.grid[neighbor4[i]] == -player && board.grid[farneighbor4[i]] == NULL_COORD) {
//                v_jump_single_target_moves.push_back(farneighbor4[i]);
//                flag = true;
//            } else if(board.grid[neighbor4[i]]==NULL_COORD){
//                v_jump_single_target_moves.push_back(neighbor4[i]);
//                flag = true;
//            }
//        }else if(Coordinate::OnBoard(neighbor4[i]) && !Coordinate::OnBoard(farneighbor4[i]) && board.grid[neighbor4[i]] == NULL_COORD){
//            v_jump_single_target_moves.push_back(neighbor4[i]);
//            flag = true;
//        }
//    }
//    return flag;
//}
//
//
//void GameState::get_player_moves(std::unordered_map<coord_t,std::vector<std::vector<coord_t>>>& v_moves) const{
//
//    for(coord_t coord:Coordinates){
//        std::vector<coord_t> v_jump_single_target_moves;
//        if(board.grid[coord] == player && is_move(coord,v_jump_single_target_moves)){
//            std::vector<std::vector<coord_t>> v_sum_moves;
//            for(auto it:v_jump_single_target_moves){
//                std::vector<coord_t> tmp;
//                tmp.push_back(it);
//                v_sum_moves.push_back(tmp);
//                std::vector<coord_t>().swap(tmp);
//            }
////            v_sum_moves.push_back(v_jump_single_target_moves);
//            v_moves[coord] = v_sum_moves;
//        }
//    }
//}
//
//void GameState::find_jump_points(const coord_t& coord,std::vector<coord_t>& v_jump) const{
//    std::vector<coord_t> neighbor4(AllNeighbor4(coord).begin(), AllNeighbor4(coord).end());
//    std::vector<coord_t> farneighbor4(AllFarNeighbor4(coord).begin(), AllFarNeighbor4(coord).end());
////    neighbor4.assign(AllNeighbor4(coord).begin(), AllNeighbor4(coord).end());
////    farneighbor4.assign(AllFarNeighbor4(coord).begin(), AllFarNeighbor4(coord).end());
//    for (size_t i = 0; i < neighbor4.size(); ++i) {
//        if (Coordinate::OnBoard(neighbor4[i]) && Coordinate::OnBoard(farneighbor4[i])) {
//            if (board.grid[neighbor4[i]] == -player && board.grid[farneighbor4[i]] == NULL_COORD &&
//                !Coordinate::m_jump[farneighbor4[i]]) {
//                v_jump.push_back(farneighbor4[i]);
//            }
//        }
//    }
//    std::vector<coord_t>().swap(neighbor4);
//    std::vector<coord_t>().swap(farneighbor4);
//}
//
//
//void GameState::get_player_jump_target_moves(const coord_t& coord) {
//    /**
//     * jumps: 跳了一下的数据
//     * v_jump_target_moves: 需要返回的数据
//     */
//     std::vector<coord_t> v_jump;
//     find_jump_points(coord,v_jump); // 26-[28]    28=true   dict= {26,[28],17,[19],19[21] }
//     if(!v_jump.empty()){
//         jump_dict.insert(std::make_pair(coord,v_jump));
//         for(auto iter:v_jump){
//             // 修改m_jump
//             Coordinate::m_jump[iter] = true;
////             jump_dict.insert(std::make_pair(coord,v_jump));
////             if(!jump_dict.count(iter)){
//                 get_player_jump_target_moves(iter);
////             }else{
////                 if(jump_dict[coord].size()==1){
////                     jump_dict.erase(coord);
////                     return;
////                 }else{
////                     jump_dict[coord].erase(remove(jump_dict[coord].begin(), jump_dict[coord].end(),iter),jump_dict[coord].end());
////                     return;
////                 }
////             }
//         }
//         return;
//    }
//    std::vector<coord_t>().swap(v_jump);
//     return;
//}
//
//void GameState::parse_points(const coord_t& coord,std::vector<std::vector<coord_t>>& v_jumps){
//    std::vector<coord_t> stack;
////    if(!stack.empty()){
////        stack.clear();
////    }
//    stack.push_back(coord);
//    std::vector<coord_t> path_st;
////    if(!path_st.empty()){
////        path_st.clear();
////    }
////    stack.clear();
////    path_st.clear();
////    std::vector<coord_t> result;
////    std::cout<<"jump-dict: "<<jump_dict.size()<<std::endl;
////    if(jump_dict.size()==5){
////        std::cout<<std::endl;
////    }
//    while (!stack.empty()){
//        coord_t cur = stack[stack.size()-1];
//        if (cur == -1) {
//            stack.pop_back();
//            if(!path_st.empty()){
//                path_st.pop_back();
//            }
//            continue;
//        }else{
//            stack.pop_back();
//            path_st.push_back(cur);
//        }
//        if (jump_dict.find(cur)==jump_dict.end()){
//            std::vector<coord_t> pa(path_st.begin(), path_st.end());
//            v_jumps.push_back(pa);
//            path_st.pop_back();
//        }
//        if (jump_dict.find(cur)!=jump_dict.end()){
//            std::vector<coord_t> cur_child(jump_dict[cur].begin(), jump_dict[cur].end());
////            if(!cur_child.empty()){
////             std::cout<<"cur_child_size: "<<cur_child.size()<<std::endl;
//                cur_child.push_back(-1);
////                reverse(cur_child.begin(),cur_child.end());
//                for(auto i=cur_child.rbegin();i!=cur_child.rend();++i){
//                    stack.push_back(*i);
//                }
////            }
////            cur_child.clear();
////            std::vector<coord_t>().swap(cur_child);
//        }
//    }
////    std::cout<<"stack end"<<std::endl;
////    if(!jump_dict.empty()){
//       jump_dict.clear();
////    }
////    stack.clear();
////    path_st.clear();
//    std::vector<coord_t>().swap(stack);
//    std::vector<coord_t>().swap(path_st);
//}
//void GameState::get_player_jump_plus_moves(const coord_t& coord,std::vector<std::vector<coord_t>>& v_jumps){
////    jump_dict.clear();
////     if(!jump_dict.empty()){
////        jump_dict.clear();
////     }
//    get_player_jump_target_moves(coord);
//
//    Coordinate::Reset_m_jump();
////    std::cout<<"row="<<std::to_string(std::get<1>(Coordinate::GetXY(coord)))<<"col="<<std::to_string(std::get<0>(Coordinate::GetXY(coord)))<<std::endl;
//    if(jump_dict.size()>1){
//        parse_points(coord, v_jumps);
//    }
//
//}
//
//
//
//
//
//
//std::vector<coord_t> GameState::legal_moves_stage1() const {
//    std::vector<coord_t> cans;
//    for (coord_t coord = 0; coord < Coordinate::m_square; ++coord) {
//            if (is_valid_move_stage1(Move::PlayStage1(coord))) {
//                cans.push_back(coord);
//            }
//
//    }
//    return cans;
//}
//
//
//void GameState::mcts_legal_moves_stage2(std::unordered_map<coord_t, std::vector<std::vector<coord_t>>>& v_moves) {
//
//    get_player_moves(v_moves);
//
////    std::vector<coord_t> v_moves_first;
//    // 头坐标
////    for (auto it:v_moves) {
////        v_moves_first.push_back(it.first);
////    }
//    // 根据头坐标插入链条路径
//    for(auto item:v_moves){
//        get_one_move_jump_plus(item.first, v_moves);
//    }
//
//}
//
//void GameState::legal_moves_stage3(std::unordered_map<coord_t, std::vector<std::vector<coord_t>>>& v_moves) {
//    get_player_moves(v_moves);
//
//    for(auto v_move:v_moves){
//        for (coord_t coord = 0; coord < Coordinate::m_square; ++coord) {
//            if (is_valid_move_stage3(coord)) {
//
//                std::vector<coord_t> tmp;
//                tmp.push_back(coord);
//                v_move.second.push_back(tmp);
//
//            }
//        }
//
//    }
//
//
//
//}
//
//
//
//void GameState::random_legal_moves_stage2(coord_t& move_first,std::unordered_map<coord_t, std::vector<std::vector<coord_t>>>& v_moves) {
//    // 得到这个coord的条吃or single 并获得一个随机的move first，根据move first将连跳加进去。
//    get_random_move_frist(move_first,v_moves);
//
//    get_one_move_jump_plus(move_first,v_moves);
//}
//
//
//void GameState::get_one_move_jump_plus(const coord_t& move_first,std::unordered_map<coord_t, std::vector<std::vector<coord_t>>>& v_moves) {
//    // 得到一个move的连跳路径并返回
////    get_random_move_frist(move_first,v_moves);
//
////    move.m_points.push_back(move_first);
//
//    std::vector<coord_t> is_jumps;
//    bool flag = false;
//    for(auto it : v_moves.find(move_first)->second){
//        find_jump_points(it[0],is_jumps);
//        if(!is_jumps.empty()){
//            flag = true;
//        }
//    }
//    if(flag){
//        // 找链条
//        std::vector<std::vector<coord_t>> v_jumps; // 连跳返回来的数据
//        // 找链条
//        get_player_jump_plus_moves(move_first, v_jumps);
//
//        if (!v_jumps.empty()) {
//            for (auto item:v_jumps) {
//                if (Coordinate::m_distance[item[item.size() - 1]][item[item.size() - 2]] == 2.0) {
//                    v_moves.find(move_first)->second.push_back(item);
//                }
//            }
//        }
//        std::vector<std::vector<coord_t>>().swap(v_jumps);
//    }else{
//        std::vector<coord_t>().swap(is_jumps);
//    }
//}
//
//
//bool GameState::is_over_stage1(GameState& game) const {
//    if(legal_moves_stage1().empty()){
//        Board next_board = board;
//        int row = static_cast<int>(Coordinate::m_size/2);
//        int col = static_cast<int>(Coordinate::m_size/2)-1;
//        coord_t coord1 = Coordinate::Coord(row,col);
//        coord_t coord2 = Coordinate::Coord(col,row);
//        auto item1 = next_board.grid[coord1];
//        auto item2 = next_board.grid[coord2];
//        if(item1== BLACK || item1 == WHITE){
//            next_board.grid[coord1] = NULL_COORD;
//        }
//        if(item2== BLACK || item2 == WHITE){
//            next_board.grid[coord2] = NULL_COORD;
//        }
//        game.board = next_board;
//        game.player = WHITE;
//        game.move = Move();
//        game.status = stage2;
//        game.jump_dict = {};
//        return true;
//    }else{
//        return false;
//    }
//}
