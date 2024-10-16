#include "thirdparty/gtest/gtest.h"
#include "kingchess_c/fundamental/gamestate.h"
#include "kingchess_c/fundamental/board.h"
#include <iostream>
#include <vector>
#include "kingchess_c/fundamental/move.h"

#include "kingchess_c/fundamental/utils.h"
int main(){

    GameState game;
    GameState::new_game(5,9,game);
    memset(game.board.grid,0,sizeof(game.board.grid));
    game.board.grid[0] = 1;
    game.board.grid[18] = -1;
    game.board.grid[36] = -1;
    game.board.grid[10] = -1;
    game.board.grid[20] = -1;

    print_board(game.board);
    std::vector<Move> moves;
    game.legal_moves(moves);

    std::cout<<moves.size()<<std::endl;


}
