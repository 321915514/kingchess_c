//
// Created by philxling on 2024/4/13.
//

#include "collect.h"
//#include "thirdparty/gflags/gflags.h"
#include "thirdparty/glog/logging.h"

//DEFINE_int32(simulate,1200,"");
//DEFINE_int32(thread,16,"");

int main(int argc,char** argv){

    google::InitGoogleLogging(argv[0]);

    google::SetLogDestination(google::INFO, "/home/test4/null");
    google::SetLogDestination(google::WARNING, "/home/test4/null");
    google::SetLogDestination(google::ERROR, "/home/test4/log/logerror");

    google::ParseCommandLineFlags(&argc, &argv, true);

    //Collect collect(1,FLAGS_simulate,FLAGS_thread); // prar 0 is temp,para 1 is sumulate, para2 is thread;
    while(true){
        //Collect collect(1,FLAGS_simulate,FLAGS_thread); // prar 0 is temp,para 1 is sumulate, para2 is thread;
        
        Collect collect(5,300,16); // prar 0 is c_puct,para 1 is sumulate, para2 is thread;
	//LOG(ERROR)<<"start self play";
        collect.self_play(false);
        //LOG(ERROR)<<"end self play";
    }
    google::ShutDownCommandLineFlags();

}
