//
// Created by philxling on 2024/3/30.
//

#include "deep_model_45.h"
#include "common.h"
int main(){

    deep_model model("/home/test4/new_kingchess/net/supervise_model/current_best_black.engine");
    float grid[45*5] = {0};
    for(int i=0;i<10;i++){

        model.infer(grid);
        model.print_infer_out();
    }
//
    //model.print_infer_out();

}
