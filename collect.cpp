//
// Created by dev on 4/9/24.
//


#include "collect.h"
#include "fundamental/utils.h"
#include "fundamental/coordinate.h"
#include <cassert>
#include <fstream>
#include "thirdparty/glog/logging.h"
#include "thirdparty/gflags/gflags.h"

DEFINE_string(game_save_dir,"/home/test4/kingchess_remote/new_kingchess/net/dataset_cpp/","path of play game file that will be training");


DEFINE_string(game_save_dir_test,"/home/test4/new_kingchess/kingchess_c/net/dataset/","path of play game file that will be training");

DEFINE_string(engine_path,"/home/test4/kingchess_remote/new_kingchess/net/board_9_model/current.engine","TensorRT engine file path");

    //std::string filename1 = "/home/test4/kingchess_remote/kingchess/net/dataset/"+file;


Collect::Collect(const double& c_puct,const int& simulate_num,const int& thread_num ):c_puct(c_puct),simulate_num(simulate_num),thread_num(thread_num){
    model.InitModel(FLAGS_engine_path);
};







std::vector<std::tuple<std::array<float,81*32>,std::vector<float>,int>> Collect::symmetry(std::array<float,81*32>& state, std::vector<float>& move_prob,const int& current_player){
    //
    std::vector<std::tuple<std::array<float,81*32>,std::vector<float>,int>> results;

    //  遍历 state state为81*5 408
    //

    //std::cout<<"symmetry"<<std::endl;

    // 先把
    //float* dimArray[5];
    //for (int i = 0; i < 5; ++i) {
    //    dimArray[i] = &state[i * 81];
    //}

    //std::cout<<"sym";

    for(size_t code=0;code<8;++code){
        // 现在可以使用twoDArray[row][col]来访问元素了
        for (int row = 0; row < 32; ++row) {
            //if(row!=2){
                float temp[81]={0};
                for (int col = 0; col < 81; ++col) {

                    //auto index = 81*row+col;

                    temp[col]  = state[ row*81 + Coordinate::m_symmetry[(81*row+col)%81][code]]; // 返回
                }

                for(auto index=row*81;index<(row*81)+81;++index){
                    state[index] = temp[index%81];
                }

                //memcpy(dimArray[row],temp,sizeof(temp));
           // }
        }
        // 修改 move_prob
//        std::vector<std::pair<std::string,float>> transform_move_prob;
//        for(size_t i=0;i<move_prob.size();i++){
//            if(Move::is_down(move_prob[i].first)){
//                move_prob[i].first.m_point = Coordinate::m_symmetry[move_prob[i].first.m_point][code];
//
//                transform_move_prob.push_back(std::make_pair(Move::move2str(move_prob[i].first),move_prob[i].second));
//            }
//            else if(Move::is_go(move_prob[i].first)){
//                move_prob[i].first.m_point = Coordinate::m_symmetry[move_prob[i].first.m_point][code];
//                move_prob[i].first.m_point_ = Coordinate::m_symmetry[move_prob[i].first.m_point_][code];
//                transform_move_prob.push_back(std::make_pair(Move::move2str(move_prob[i].first),move_prob[i].second));
//            }

//        }
        //std::cout<<"end symmtery"<<std::endl;
        results.push_back(std::make_tuple(state,move_prob,current_player));

    }

    return results;

}





std::vector<std::tuple<std::array<float,81*32>,std::vector<float>,int>> Collect::get_equi_data(std::vector<std::array<float,81*32>> states,  std::vector<std::vector<float>> move_probs, std::vector<int> current_players){
    // 继续
    /*
     * state 5*81
     * move_probs (string,float)
     * currwnt_players  int
     */

    //std::cout<<states.size()<<"---"<<move_probs.size()<<"--"<<current_players.size()<<std::endl;

    assert(states.size() == move_probs.size() == current_players.size());

    std::vector<std::tuple<std::array<float,81*32>,std::vector<float>,int>> play_data;

    for(auto i=0;i<states.size();++i){
        //
        auto symmetries = symmetry(states[i],move_probs[i],current_players[i]); // return std::vector<std::tuple<float*,std::vector<std::pair<std::string,float>>,int>>

        //std::cout<<symmetries.size()<<std::endl;

        play_data.insert(play_data.end(),symmetries.begin(),symmetries.end());
    }

    return play_data;

}

void Collect::write_proto_file(std::vector<std::tuple<std::array<float,81*32>,std::vector<float>,int>>& play_data,const bool& show){

    TripletArray triplet_array;
    // 在此处填充你的 TripletArray 实例数据

    for (auto i=0;i<play_data.size();i++) {
        Triplet triplet = Triplet();

        for (auto j = 0; j < 81*32; j++) {
            triplet.add_array(static_cast<int32_t>(std::get<0>(play_data[i])[j]));
        }
	
	//for(auto k=0; k<1125; k++){
        //   triplet.add_dictionary(static_cast<float>(std::get<1>(play_data[i][k])));
	//}

        for (const auto& item : std::get<1>(play_data[i])){
	
           triplet.add_dictionary(static_cast<float>(item));
            //Stone move;
            //move.set_point(item.first);
            //move.set_prob(item.second);
        //    *triplet.add_dictionary() = move;
       }
        triplet.set_value(std::get<2>(play_data[i]));
        *triplet_array.add_triplets() = triplet;

    }

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    char filename[80];
    //LOG(ERROR)<<"get filename";
    sprintf(filename, "file_%d-%02d-%02d-%02d-%02d-%02d.game",
            tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
            tm.tm_hour, tm.tm_min, tm.tm_sec);
    LOG(ERROR)<<"Current file_name:"<<filename;

//    std::string file(filename);
//    auto  filename1 = file+".game";


    std::string file(filename);

    //std::string filename1 = "/home/dev/kingchess_remote/kingchess/net/dataset/"+file;
    //
    std::string filename1;
    if(show){
         filename1 = FLAGS_game_save_dir_test+file;
    }else{
         filename1 = FLAGS_game_save_dir+file;
    }
    //std::string filename1 = "/home/test4/tmp/kingchess/net/dataset/"+file;

// 将消息序列化到文件
    std::fstream output(filename1, std::ios::out | std::ios::binary | std::ios::trunc);
    if (!triplet_array.SerializeToOstream(&output)) {
        LOG(ERROR) << "Failed to write triplet array." << std::endl;
        return;
    }
    LOG(ERROR) <<filename<< " write." << std::endl;
}



void Collect::self_play(const bool& show) {

    //cudaSetDevice(8);
    deep_model model(FLAGS_engine_path);
    auto start = std::chrono::high_resolution_clock::now();

   // cudaSetDevice(8);
   // cudaSetDevice(8);
    //deep_model model(FLAGS_engine_path);

    std::vector<std::array<float,81*32>> states;
    std::vector<std::vector<float>> move_probs;
    std::vector<int> current_players;

    GameState game;
    GameState::new_game(5,9,game);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0,1);
   
    if(dis(gen)<0.5){
	//int count_chess=0;
   	//for(size_t i=0;i<45;i++){
    	 //    if(game.board.grid[i]==0 && Coordinate::Legal_coord(i) && dis(gen) && count_chess<=16){
	 //   	     game.board.grid[i] = -1;
	  // 	     count_chess++;
	//	}
	 // }
	 //
	 //
	int emptyCount = 37;
    	int count_chess = 0;
        int targetCount = rand() % 7 + 10;

	std::vector<int> possibleIndices;
	for (int i = 0; i < 45; ++i) {
		if (game.board.grid[i] == 0 && Coordinate::Legal_coord(i)) {
		    possibleIndices.push_back(i);
		}
	}
	while (count_chess < targetCount) {
	    int randomIndex = rand() % possibleIndices.size();
	    int indexToModify = possibleIndices[randomIndex];
	    game.board.grid[indexToModify] = -1;
	    possibleIndices.erase(possibleIndices.begin() + randomIndex);
	    count_chess++;
	    }
	}else{
	    int less_chess = 8;
	    for(size_t i=0;i<45;i++){
	       if(game.board.grid[i]== -1 && dis(gen) && less_chess>=3){
	             game.board.grid[i] = 0;
	   	     less_chess--;
	       } 
	    }
	}
     if(dis(gen)<0.2){
     	if(dis(gen)<0.5){
	    game.board.grid[20] = 0;
	}else{
	    game.board.grid[24] = 0;
	}
     } 
 
    std::vector<Move> moves;

    //game.legal_moves(moves);

    auto mcts = MCTS(&model, thread_num, c_puct, simulate_num, 0.3);

    float grid[81*32]={0};
    int winner = 0;
    std::array<float,81*32> arr{0.0f};
    while (true){
        if(show){
            print_board(game.board);
        }
        moves.clear();
        game.legal_moves(moves);
        if (moves.size() == 0){
            game.move.m_point = -100;
            game.move.m_point_ = -100;
            //winner = 0;
           // game.is_gameover(winner);
           // LOG(ERROR)<<"winner:"<<winner;
        }
        winner = 0;
        game.is_gameover(winner);
        bool full_search = false;
        //std::cout<<winner<<std::endl;
        if(winner !=0){
            // 结束
            std::vector<int> winner_z(current_players.size(),0);
            //if(winner != -1){
                for(size_t i=0;i<current_players.size();i++){
                    if(current_players[i] == winner){
                        winner_z[i] = 1.0;
                    }else{
                        winner_z[i] = -1.0;
                    }
                }
            //}
           // mcts.update_with_move(-1);
            //
            auto play_data = get_equi_data(states,move_probs,winner_z);
            //std::cout<<"get play_data";
            write_proto_file(play_data,show);

            LOG(ERROR)<<"game state length:"<<states.size();

            auto end = std::chrono::high_resolution_clock::now();
            // 计算时间间隔
            std::chrono::duration<double> duration = end - start;
            // 输出时间间隔
            LOG(ERROR) << "speed time " << duration.count() << "s";

            break;
        }

        auto game_copy = GameState(game);
        //std::cout<<"run mcts"<<std::endl;

        auto mcts_start = std::chrono::high_resolution_clock::now();

        //if(!this->full_search()){ // full_search true is 0.75 flase is 0.25
        //    full_search = true;
        //}else{
        //    full_search = false;
        //}
        auto probs = mcts.get_action_probs(&game_copy,show, 5);

	
	//for(auto it:probs){
	//	std::cout<<it<<",";
	
	//}	
	

        if(show){
            //LOG(ERROR)<<"game state length:"<<states.size();

            auto mcts_end = std::chrono::high_resolution_clock::now();
            // 计算时间间隔
            std::chrono::duration<double> duration = mcts_end - mcts_start;
            // 输出时间间隔
            LOG(ERROR) << "mcts speed time " << duration.count() << "s";
        }


        //std::cout<<"end mcts"<<std::endl;
        mcts.update_with_move(-1);

        std::unordered_map<int,double> actions;
        for(auto item:moves){
	    auto action = game.move_2_action(item);
            actions[action] = probs[action];
        }
	

       	auto act = combineProbabilities(actions);


        std::vector<float> move_prob;
        for(size_t i =0;i<1125;i++){
            move_prob.push_back(probs[i]); // move2str
            if(show){
                //LOG(ERROR)<<Move::move2str(moves[i])<<"--"<<probs[i]<<"--"<<add_n[i];
            }
        }

            auto game_encoder = std::make_shared<GameState>(game);

            memset(grid,0,sizeof(grid));

            encoder_data(game_encoder,grid);

            //std::array<float,81*5> arr;

            memcpy(arr.data(),grid,sizeof(grid));

            states.push_back(arr);
            // add random nosie

            //LOG(ERROR)<<"probs size:"<<probs.size()<<" legal moves:"<<moves.size();

            //auto add_n = this->combineProbabilities(probs);
            //std::vector<std::pair<Move,float>> move_prob;
            //for(size_t i =0;i<moves.size();i++){
            //    move_prob.push_back(std::make_pair(moves[i],probs[i])); // move2str
            //    if(show){
            //        LOG(ERROR)<<Move::move2str(moves[i])<<"--"<<probs[i]<<"--"<<add_n[i];
            //    }
            //}
            //LOG(ERROR)<<game.play_out;
            //LOG(ERROR)<<game.eat_point;
          //  int best_action = -1;
          //  for(int i=0; i<actions.size(); i++){
          //      if(probs[actions[i]]>best_action){
          //          best_action = i;
          //      }
          //  }
            move_probs.emplace_back(move_prob);
            current_players.push_back(game.player);
        
        //auto index = find_max_index(probs.size());
	
	//show
	
        if(show){
            for(auto it:moves){
                LOG(ERROR)<<"move:"<<Move::move2str(it)<<":"<<probs[game.move_2_action(it)];
            }
	    
	    LOG(ERROR)<<"apply act:"<< act;
	    LOG(ERROR)<<"apply move:"<< Move::move2str(game.action_2_move(act));

            LOG(ERROR)<<"legal moves:"<<moves.size();

            LOG(ERROR)<<game.play_out;

            LOG(ERROR)<<game.eat_point;

        }

        // show end

         //auto best_action = get_random_index(moves.size());
       // LOG(ERROR) << Move::move2str(moves[index]);
        game.apply_move(game.action_2_move(act), game);
    }


}

