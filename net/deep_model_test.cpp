//
// Created by philxling on 2024/3/30.
//

#include "deep_model.h"
#include "common.h"
int main(){
    //cudaSetDevice(8);
    //deep_model model("/home/dev/kingchess/net/current.engine");
//    model.InitModel("/home/dev/kingchess/net/current.engine");

    deep_model model("/home/test4/new_kingchess/net/model/current.engine");
    float grid[81*5] = {0};
    for(int i=0;i<10;i++){

    //    deep_model model("/home/test4/kingchess_remote/kingchess/net/model/current.engine");
        model.infer(grid);
        model.print_infer_out();
    }
//
    //model.print_infer_out();

}
