#cmake_minimum_required(VERSION 3.26)
cmake_minimum_required(VERSION 3.16)
project(kingchess_c)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_BUILD_TYPE Debug)

#set(TensorRT_ROOT E:/TensorRT-8.5.1.7.Windows10.x86_64.cuda-11.8.cudnn8.6/TensorRT-8.5.1.7)

#set(TensorRT_ROOT E:/TensorRT-8.4.1.5.Windows10.x86_64.cuda-11.6.cudnn8.4/TensorRT-8.4.1.5)

#set(TensorRT_ROOT E:/TensorRT-8.6.1.6.Windows10.x86_64.cuda-11.8/TensorRT-8.6.1.6)

#set(CUDA_TOOLKIT_ROOT_DIR "D:/Program Files/NVIDIA GPU Computing Toolkit")
#include_directories(CUDA_INCLUDE_DIRS "D:/Program Files/NVIDIA GPU Computing Toolkit/include")
#include_directories(CUDA_LIBRARIES "D:/Program Files/NVIDIA GPU Computing Toolkit/lib/x64")

find_package(CUDA REQUIRED)
message(STATUS "CUDA Libs: \n${CUDA_LIBRARIES}\n")
#get_filename_component(CUDA_LIB_DIR ${CUDA_LIBRARIES} DIRECTORY)
message(STATUS "CUDA Headers: \n${CUDA_INCLUDE_DIRS}\n")


set(TensorRT_INCLUDE_DIRS /usr/local/cuda/include)
set(TensorRT_LIBRARIES /usr/local/cuda/lib64)




#include_directories("D:/TensorRT-8.4.1.5.Windows10.x86_64.cuda-11.6.cudnn8.4/TensorRT-8.4.1.5/include")
#include_directories(CUDA_INCLUDE_DIRS "D:/Program Files/NVIDIA GPU Computing Toolkit/include")
#include_directories(CUDA_LIBRARIES "D:/Program Files/NVIDIA GPU Computing Toolkit/lib/x64")

include_directories(/home/test4/tmp/kingchess_c/thirdparty/protobuf)


include_directories(/home/test4/tmp/kingchess_c/thirdparty/google)
include_directories(Protobuf_LIBRARIES /home/test4/tmp/kingchess_c/thirdparty/protobuf/lib64_release)


#include_directories(/home/test4/tmp/kingchess_c/thirdparty/protobuf/bin)


#include_directories(/home/test4/tmp/kingchess_c/thirdparty/protobuf/google/protobuf)

#add_executable(write_proto_test fundamental/train.pb.cc fundamental/train.pb.h fundamental/write_proto_test.cpp fundamental/neighbor.cpp fundamental/neighbor.h fundamental/zobrist.h fundamental/symmetry_test.cpp
#        fundamental/utils.h fundamental/utils.cpp  fundamental/board.cpp fundamental/gamestate.cpp fundamental/coordinate.cpp fundamental/coordinate.h fundamental/board.h fundamental/gamestate.h fundamental/agent.h fundamental/agent.cpp)


add_executable(hello fundamental/hello.cpp fundamental/board.h fundamental/board.cpp fundamental/gamestate.h fundamental/gamestate.cpp fundamental/move.h fundamental/move.cpp fundamental/coordinate.h fundamental/coordinate.cpp fundamental/neighbor.h fundamental/neighbor.cpp fundamental/zobrist.h)

add_executable(symmetry_test fundamental/neighbor.cpp fundamental/neighbor.h fundamental/zobrist.h fundamental/symmetry_test.cpp
        fundamental/utils.h fundamental/utils.cpp  fundamental/board.cpp fundamental/gamestate.cpp fundamental/coordinate.cpp fundamental/coordinate.h fundamental/board.h fundamental/gamestate.h fundamental/agent.h fundamental/agent.cpp)


#
add_executable(bot_vs_bot_random fundamental/neighbor.cpp fundamental/neighbor.h fundamental/zobrist.h fundamental/bot_vs_bot_random.cpp
        fundamental/utils.h fundamental/utils.cpp  fundamental/board.cpp fundamental/gamestate.cpp fundamental/coordinate.cpp fundamental/coordinate.h fundamental/board.h fundamental/gamestate.h fundamental/agent.h fundamental/agent.cpp)


add_executable(encoder_test fundamental/zobrist.h fundamental/neighbor.cpp fundamental/neighbor.h fundamental/board.cpp fundamental/board.h fundamental/move.h fundamental/move.cpp fundamental/encoder_test.cpp fundamental/gamestate.cpp fundamental/gamestate.h fundamental/coordinate.cpp fundamental/coordinate.h fundamental/utils.h fundamental/utils.cpp)

#add_executable(bot_vs_bot_random_simple fundamental/neighbor.cpp fundamental/neighbor.h fundamental/zobrist.h fundamental/bot_vs_bot_random_simple.cpp
#        fundamental/utils.cpp  mcts/rw_lock.h fundamental/board.cpp fundamental/gamestate.cpp fundamental/coordinate.cpp fundamental/coordinate.h fundamental/board.h fundamental/gamestate.h fundamental/agent.h)


#add_executable(bot_vs_bot_mcts fundamental/neighbor.cpp fundamental/neighbor.h fundamental/zobrist.h fundamental/bot_vs_bot_mcts.cpp mcts/mcts.h mcts/mcts.cpp mcts/node.cpp mcts/node.h
#         fundamental/utils.h  fundamental/utils.cpp fundamental/agent.h  fundamental/agent.cpp  fundamental/board.cpp fundamental/gamestate.cpp fundamental/coordinate.cpp fundamental/coordinate.h fundamental/board.h fundamental/gamestate.h
#        mcts/rw_lock.h)



add_executable(bot_vs_bot_mcts_pure fundamental/neighbor.cpp fundamental/neighbor.h fundamental/zobrist.h fundamental/bot_vs_bot_mcts_pure.cpp  net/mcts_pure.h net/mcts_pure.cpp
         fundamental/utils.h  fundamental/utils.cpp fundamental/agent.h  fundamental/agent.cpp  fundamental/board.cpp fundamental/gamestate.cpp fundamental/coordinate.cpp fundamental/coordinate.h fundamental/board.h fundamental/gamestate.h
      )


add_executable(mcts_pure_test fundamental/neighbor.cpp fundamental/neighbor.h fundamental/zobrist.h fundamental/mcts_pure_test.cpp  net/mcts_pure.h net/mcts_pure.cpp
        fundamental/utils.h  fundamental/utils.cpp fundamental/agent.h  fundamental/agent.cpp  fundamental/board.cpp fundamental/gamestate.cpp fundamental/coordinate.cpp fundamental/coordinate.h fundamental/board.h fundamental/gamestate.h
)


add_executable(bot_vs_human fundamental/neighbor.cpp fundamental/neighbor.h fundamental/zobrist.h fundamental/bot_vs_human.cpp
        fundamental/utils.h fundamental/utils.cpp  fundamental/board.cpp fundamental/gamestate.cpp fundamental/coordinate.cpp fundamental/coordinate.h fundamental/board.h fundamental/gamestate.h fundamental/agent.h fundamental/agent.cpp)

add_executable(mcts_test net/common.h  fundamental/neighbor.cpp fundamental/neighbor.h fundamental/zobrist.h net/mcts.cpp net/mcts.h net/mcts_test.cpp net/deep_model.h net/deep_model.cpp
        fundamental/utils.h fundamental/utils.cpp fundamental/move.cpp fundamental/move.h  fundamental/board.cpp fundamental/gamestate.cpp fundamental/coordinate.cpp fundamental/coordinate.h fundamental/board.h fundamental/gamestate.h fundamental/agent.h fundamental/agent.cpp)



#add_executable(bot_vs_bot_mcts_without_stage3 fundamental/neighbor.cpp fundamental/neighbor.h fundamental/zobrist.h fundamental/bot_vs_bot_mcts_without_stage3.cpp mcts/node.h mcts/node.cpp mcts/mcts_without_stage3.cpp mcts/mcts_without_stage3.h
#        fundamental/utils.cpp fundamental/agent.h fundamental/bot_vs_bot_random_simple_mcts_without_stage3.h  fundamental/board.cpp fundamental/gamestate.cpp fundamental/coordinate.cpp fundamental/coordinate.h fundamental/board.h fundamental/gamestate.h
#        mcts/rw_lock.h fundamental/bot_vs_bot_random_simple_mcts_without_stage3.cpp)





#add_executable(bot_vs_bot_random_without_stage3 fundamental/neighbor.cpp fundamental/neighbor.h fundamental/zobrist.h fundamental/bot_vs_bot_random_without_stage3.cpp
#        fundamental/utils.cpp mcts/rw_lock.h fundamental/board.cpp fundamental/gamestate.cpp fundamental/coordinate.cpp fundamental/coordinate.h fundamental/board.h fundamental/gamestate.h fundamental/agent.h)
#
#add_executable(bot_vs_bot_mcts_vs_random fundamental/neighbor.cpp fundamental/neighbor.h fundamental/zobrist.h fundamental/bot_vs_bot_mcts_vs_random_test.cpp mcts/node.h mcts/node.cpp mcts/mcts_without_stage3.cpp mcts/mcts_without_stage3.h
#        fundamental/utils.cpp fundamental/agent.h fundamental/bot_vs_bot_random_simple_mcts_without_stage3.h  fundamental/board.cpp fundamental/gamestate.cpp fundamental/coordinate.cpp fundamental/coordinate.h fundamental/board.h fundamental/gamestate.h
#        mcts/rw_lock.h fundamental/bot_vs_bot_random_simple_mcts_without_stage3.cpp)



add_executable(test_board fundamental/utils.cpp fundamental/test_board.cpp fundamental/neighbor.h  fundamental/board.h fundamental/board.cpp fundamental/coordinate.h fundamental/neighbor.cpp fundamental/coordinate.cpp)

#add_executable(test1 mcts/rw_lock.h  fundamental/utils.h fundamental/utils.cpp fundamental/neighbor.h fundamental/neighbor.cpp fundamental/board.cpp fundamental/move.cpp fundamental/move.h fundamental/gamestate.cpp fundamental/board.h fundamental/test1.cpp fundamental/coordinate.cpp fundamental/coordinate.h fundamental/agent.h fundamental/agent.cpp fundamental/gamestate.h)
#
#
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
#
#
#add_executable(demo_mcts_thread fundamental/demo_mcts_thread.cpp  fundamental/neighbor.cpp fundamental/neighbor.h fundamental/zobrist.h   mcts/node_thread.cpp mcts/node_thread.h
#        fundamental/utils.cpp fundamental/agent.h fundamental/agent.cpp  fundamental/board.cpp fundamental/gamestate.cpp fundamental/coordinate.cpp fundamental/coordinate.h fundamental/board.h fundamental/gamestate.h mcts/rw_lock.h)

#
#add_executable(bot_vs_bot_mcts_thread fundamental/bot_vs_bot_mcts_thread.cpp  fundamental/neighbor.cpp fundamental/neighbor.h fundamental/zobrist.h   mcts/node_thread.cpp mcts/node_thread.h mcts/mcts_without_stage3_thread_new.cpp mcts/mcts_without_stage3_thread_new.h
#        fundamental/utils.cpp fundamental/agent.h fundamental/bot_vs_bot_random_simple_mcts_without_stage3.h  fundamental/board.cpp fundamental/gamestate.cpp fundamental/coordinate.cpp fundamental/coordinate.h fundamental/board.h fundamental/gamestate.h
#        fundamental/bot_vs_bot_random_simple_mcts_without_stage3.cpp mcts/rw_lock.h)



#add_executable(theradpool_test mcts/theradpool_test.cpp mcts/threadpool.cpp mcts/threadpool.h)

list(APPEND INCLUDE_DIRS
        ${CUDA_INCLUDE_DIRS}
#        ${OpenCV_INCLUDE_DIRS}
        ${TensorRT_INCLUDE_DIRS}
        include
)

list(APPEND ALL_LIBS
        ${CUDA_LIBRARIES}
        ${CUDA_LIB_DIR}
#        ${OpenCV_LIBRARIES}
        ${TensorRT_LIBRARIES}
)

include_directories(${INCLUDE_DIRS})

#find_package(Protobuf REQUIRED)
#include_directories(${Protobuf_INCLUDE_DIRS})
#include_directories(${CMAKE_CURRENT_BINARY_DIR})
#protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS train.proto)
#protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS EXPORT_MACRO DLL_EXPORT train.proto)


#target_link_libraries(write_proto_test ${Protobuf_LIBRARIES})


add_executable(deep_model_test  net/deep_model_test.cpp net/common.h net/deep_model.h net/deep_model.cpp)
target_link_directories(deep_model_test PUBLIC ${ALL_LIBS})

message("${ALL_LIBS}")
target_link_libraries(deep_model_test PRIVATE nvinfer nvinfer_plugin cudart)

target_link_directories(mcts_test PUBLIC ${ALL_LIBS})
target_link_libraries(mcts_test PRIVATE nvinfer nvinfer_plugin cudart pthread)




#target_link_libraries(deep_model_test nvinfer)
#target_link_libraries(deep_model_test ${CUDA_LIBRARIES})
