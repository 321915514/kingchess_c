cc_library(name='collect',srcs='collect.cpp',deps=['//kingchess_c/net:deep_model','//kingchess_c/net:mcts','//kingchess_c/fundamental:train_proto','//thirdparty/glog:glog','//thirdparty/gflags:gflags'])



cc_library(name='collect_45',srcs='collect_45.cpp',deps=['//kingchess_c/net:deep_model_45','//kingchess_c/net:mcts_45','//kingchess_c/fundamental:train_proto','//thirdparty/glog:glog','//thirdparty/gflags:gflags'])







cc_binary(name='collect_main',srcs='collect_main.cpp',deps=[':collect','//kingchess_c/fundamental:coordinate','//thirdparty/glog:glog','//thirdparty/gflags:gflags'])



cc_binary(name='collect_main_45',srcs='collect_main_45.cpp',deps=[':collect_45','//kingchess_c/fundamental:coordinate','//thirdparty/glog:glog','//thirdparty/gflags:gflags'])


cc_binary(name='augment_data_test',srcs='augment_data_test.cpp',deps=[':collect_45'])

cc_binary(name='collect_main_muc',srcs='collect_main_muc.cpp',deps=[':collect'])

package(name='collect_main_package',type="tgz",srcs='collect_main.cpp',deps=[':collect','//kingchess_c/fundamental:coordinate','//thirdparty/glog:glog','//thirdparty/gflags:gflags'])


cc_binary(name='model_vs_alpha',srcs='model_vs_alpha.cpp',deps=['//kingchess_c/fundamental:neighbor','//kingchess_c/fundamental:coordinate','//kingchess_c/fundamental:gamestate','//kingchess_c/fundamental:utils','//kingchess_c/fundamental:alpha_beta','//kingchess_c/fundamental:expert','//kingchess_c/net:deep_model_45','//kingchess_c/net:mcts_45_expert'])




cc_binary(name='resnet_vs_vit',srcs='resnet_vs_vit.cpp',deps=['//kingchess_c/fundamental:neighbor','//kingchess_c/fundamental:coordinate','//kingchess_c/fundamental:gamestate','//kingchess_c/fundamental:utils','//kingchess_c/fundamental:alpha_beta','//kingchess_c/fundamental:expert','//kingchess_c/net:deep_model_45','//kingchess_c/net:mcts_45_expert'])



