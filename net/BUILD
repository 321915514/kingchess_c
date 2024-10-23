cc_library (
    name = "deep_model",
    srcs = "deep_model.cpp",
    #hdrs=['deep_model.h','common.h'],
    deps=[
          '#nvinfer',
          '#cudart',
          '#cublas',
          '#cudnn',
          '#nvinfer_plugin',

    ],
    visibility = ['PUBLIC']
)


cc_library (
    name = "deep_model_45",
    srcs = "deep_model_45.cpp",
    #hdrs=['deep_model.h','common.h'],
    deps=[
          '#nvinfer',
          '#cudart',
          '#cublas',
          '#cudnn',
          '#nvinfer_plugin',

    ],
    visibility = ['PUBLIC']
)





cc_binary(name='deep_model_test',srcs='deep_model_test.cpp',
          deps=[
                  ':deep_model',
                  '#cudart',
          ]),


cc_binary(name='deep_model_test_45',srcs='deep_model_test_45.cpp',
          deps=[
                  ':deep_model_45',
                  '#cudart',
          ]),









cc_library(name='mcts',srcs='mcts.cpp',deps=[':deep_model',"#pthread",'//kingchess_c/fundamental:gamestate','//kingchess_c/fundamental:utils','//thirdparty/gflags:gflags','//thirdparty/glog:glog'],
          # hdrs=['mcts.h','threadpool.h']
          ),


cc_library(name='mcts_45',srcs='mcts_45.cpp',deps=[':deep_model_45',"#pthread",'//kingchess_c/fundamental:gamestate','//kingchess_c/fundamental:expert','//kingchess_c/fundamental:utils','//thirdparty/gflags:gflags','//thirdparty/glog:glog'],
          # hdrs=['mcts.h','threadpool.h']
          )


cc_library(name='mcts_pure',srcs='mcts_pure.cpp',deps=['#pthread','//kingchess_c/fundamental:agent' ,'//kingchess_c/fundamental:gamestate','//kingchess_c/fundamental:utils'])

cc_test(name='mcts_test',srcs='mcts_test.cpp',deps=[':deep_model_45',':mcts_45','//kingchess_c/fundamental:utils'])



cc_library(name='mcts_add_expert',srcs='mcts_add_expert.cpp',deps=['#pthread','//kingchess_c/fundamental:agent' ,'//kingchess_c/fundamental:gamestate','//kingchess_c/fundamental:utils','//kingchess_c/fundamental:expert'])

