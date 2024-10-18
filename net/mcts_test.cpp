//
// Created by dev on 4/8/24.
//
#include "mcts_45.h"
#include "deep_model.h"

int main(){
    //cudaSetDevice(8);
//    deep_model model("/home/dev/kingchess/net/current.engine");
//    model.InitModel("/home/dev/kingchess/net/current.engine");

    GameState game;
    GameState::new_game(5,9,game);

    //std::vector<Move> moves;
//
    //game.legal_moves(moves);
    deep_model model("/home/test4/new_kingchess/net/model/current.engine");

    //auto player = MCTS(3, 5, 1200, 0.3);

//    float grid[81*5] = {0};
////    game.apply_move(moves[0],game);
//    auto game_copy = std::make_shared<GameState>(game);
//
//    encoder(game_copy,grid);

//    for(auto item:grid){
//        std::cout<<item<<"\t";
//    }
//    std::cout<<std::endl;
//
//    auto result = model.commit(grid);
//
//    for(auto item:result){
//        for(auto i:item){
//            std::cout<<i<<"\t";
//        }
//        std::cout<<std::endl;
//    }

    std::cout << "RUNNING" << std::endl;

    std::vector<Move> moves;
    while (true) {
        //std::cout<<"get action"<<std::endl;


        auto player = MCTS(&model,16, 3, 1200, 0.3);

        print_board(game.board);

        auto game_copy = GameState(game);

	int winner = 0;
	game.is_gameover(winner);
	if(!winner){
        	auto res = player.get_action_probs(&game_copy,true, 1e-3);
		
		int action = 0;
		
		for(int i=0;i<res.size();i++){
			if(res[i]){
				action = i;
			}
		}
		std::string str = Move::move2str(game.action_2_move(action));
		std::cout<<str<<std::endl;
		game.apply_move(game.action_2_move(action), game);
       // std::vector<Move> moves;
       // std::cout<<"get action";
        //moves.clear();

        //game.legal_moves(moves);

        //game.apply_move(moves[get_random_index(moves.size())],game);

        //std::for_each(res.begin(), res.end(),
        //              [](double x) { std::cout << x << ","; });
        //std::cout << std::endl;
        player.update_with_move(-1);
    }else{
    	std::cout<<"game over:"<<winner<<std::endl;
	break;
    	}
    }

}
