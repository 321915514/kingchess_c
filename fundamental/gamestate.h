//
// Created by wangxu on 2023/10/18.
//


#ifndef LONGCHESS_GAMESTATE_H
#define LONGCHESS_GAMESTATE_H

#include <map>
#include "board.h"

class GameState {
public:
    Board board;
    int16_t player;
    Move move;
    int32_t play_out;
    coord_t eat_point;
//    std::map<coord_t,std::vector<coord_t>> jump_dict;
    std::vector<Move> moves;
    ~GameState(){
//        jump_dict.clear();
//
    }

    GameState(){
        board = Board();
        player = BLACK;
        move = Move();
        play_out = 0;
        eat_point = -1;
	moves = {};
//        status = Status::stage1;
//        jump_dict = {};
    };

    GameState(const Board& board, const int16_t &player, const Move& move,const coord_t& eat_point,const std::vector<Move>& moves) : board(board), player(player), move(move),eat_point(eat_point),moves(moves) {}

    GameState(const Board& board, const int16_t &player, const Move& move, const int32_t & play_out,const coord_t& eat_point,const std::vector<Move> moves) : board(board), player(player), move(move), play_out(play_out), eat_point(eat_point),moves(moves) {}


//    GameState() = default;

    // 拷贝构造函数

    GameState(const GameState &other_game_state) : board(other_game_state.board), player(other_game_state.player), move(other_game_state.move),play_out(other_game_state.play_out),eat_point(other_game_state.eat_point), moves(other_game_state.moves) {}





    // 赋值运算符重载
    GameState& operator=(const GameState& other_game) {
        if (this == &other_game) {
            return *this;
        }

        board = other_game.board;
        player = other_game.player;
        move = other_game.move;
        play_out = other_game.play_out;
        eat_point = other_game.eat_point;
	moves = other_game.moves;
//        jump_dict = other_game.jump_dict;
        return *this;
    }

    static void new_game(const int16_t& row,const int16_t& col,GameState& game) {
        game.board = Board(row, col);
        game.player = BLACK;
        game.move = Move();
        game.play_out = 0;
        game.eat_point = -1;
	game.moves = {};
//        game.jump_dict = {};
    }

    void apply_move(const Move& move,GameState& newgame) const;

    coord_t eat_chess() const;

    bool is_valid_move(const Move& move) const;


    void get_legal_moves(const std::vector<coord_t>& kings,std::vector<Move>& moves);


    void one_king_legal_move(const coord_t& king,std::vector<Move>& one_king_legal_move);

    // get all moves
    void legal_moves(std::vector<Move>& moves);


    void is_gameover(int& winner);

    int move_2_action(const Move& move);

    Move action_2_move(const int action);

    int transfer_pos(const Move& move);

    int transfer_coord(int action);

    //int move_2_action(const Move move) const;



    

//    GameState apply_move(const Move& move) const {
//        Board next_board = board;
//        if (is_valid_move(move)) {
//            next_board.place_stone(move, player);
//            return GameState(next_board,player == Player::Black?Player::White:Player::Black,move);
//        }
//        return *this;
//    }

//    GameState apply_move_one_player(const Move move,Player player1) const {
//        Board next_board = board;
//        if (is_valid_move(move)) {
//            next_board.place_stone(move.point, player1==Player::black?Player::white:Player::black);
//            return GameState(next_board, player1==Player::black?Player::white:Player::black, move);
//        }
//        return *this;
//







    /*

    under is jiuchess,wuwuwuwuwuwuwu,angry!!!!!!!!!!

    bool is_valid_move_stage1(const Move& move) const;

    bool is_valid_move_stage3(const coord_t& coord) const;

    std::vector<coord_t> legal_moves_stage1() const;

    void random_legal_moves_stage2(coord_t& move_first,std::unordered_map<coord_t, std::vector<std::vector<coord_t>>>& v_moves);

    void mcts_legal_moves_stage2(std::unordered_map<coord_t, std::vector<std::vector<coord_t>>>& v_moves);

    void legal_moves_stage3(std::unordered_map<coord_t, std::vector<std::vector<coord_t>>>& v_moves);

    void get_one_move_jump_plus(const coord_t& move_first,std::unordered_map<coord_t, std::vector<std::vector<coord_t>>>& v_moves);

    bool is_over_stage1(GameState& game) const;

    bool is_square(const coord_t& coord,coord_t& square) const;


    void get_player_moves(std::unordered_map<coord_t,std::vector<std::vector<coord_t>>>& v_moves) const;

    void get_player_jump_target_moves(const coord_t& coord);

    bool is_gameover() const;

    void parse_points(const coord_t& coord,std::vector<std::vector<coord_t>>& v_moves);

    void get_player_jump_plus_moves(const coord_t& coord,std::vector<std::vector<coord_t>>& v_moves);

    void find_jump_points(const coord_t& coord,std::vector<coord_t>& v_jump) const;

    //stage_2
    bool is_move(const coord_t& coord,std::vector<coord_t>& v_jump_single_target_moves) const;

    void get_random_move_frist(coord_t& coord,std::unordered_map<coord_t, std::vector<std::vector<coord_t>>>& v_moves) const;


    */






//    std::pair<bool, Player> check(const std::vector<Point>& win_type) const {
//        int count = 0;
//        Player player = board.get(win_type.back());
//        if (player != Player::none) {
//            // 不为空在执行
//            for (const Point& point : win_type) {
//                if (board.get(point) == player && board.is_on_grid(point)) {
//                    count++;
//                }
//            }
//            return std::make_pair(count == 3, player);
//        }
//        return std::make_pair(count == 3, player);
//    }

//    Player winner() const {
//        for (int row = 1; row <= board.num_rows; ++row) {
//            for (int col = 1; col <= board.num_cols; ++col) {
//                Point point(row, col);
//                std::vector<Point> win_types = {
//                        {point.row - 1, point.col},
//                        {point.row + 1, point.col},
//                        {point.row, point.col - 1},
//                        {point.row, point.col + 1},
//                        {point.row - 1, point.col - 1},
//                        {point.row + 1, point.col + 1},
//                        {point.row - 1, point.col + 1},
//                        {point.row + 1, point.col - 1}
//                };
//                for (size_t i = 0; i < win_types.size(); i += 2) {
//                    if (check({ win_types[i], win_types[i + 1], point }).first) {
//                        return check({ win_types[i], win_types[i + 1], point }).second;
//                    }
//                }
//            }
//        }
//        return Player::none; // Default to Player::black if no winner
//    }



};




#endif //LONGCHESS_GAMESTATE_H
