#cc_library(
#   name = 'json',
#   srcs = 'json.hpp',
#),
cc_binary(
   name = "server",
   srcs = "client.cpp",
        deps= ['//kingchess_c/fundamental:gamestate','//kingchess_c/net:mcts_45','#pthread'],
)
cc_binary(
   name = "server_new",
   srcs = "server_new.cpp",
        deps= ['//kingchess_c/fundamental:gamestate','//kingchess_c/net:deep_model','//kingchess_c/net:mcts'],
)

