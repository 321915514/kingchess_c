


cc_library(name='coordinate',srcs='coordinate.cpp',
           #hdrs=['coordinate.h'],
           #visibility=['PUBLIC']
          )
cc_library(name='neighbor',srcs='neighbor.cpp',deps=[':coordinate'],
           #hdrs=['neighbor.h'],
          # visibility=['PUBLIC']
          )
cc_library(name='move',srcs='move.cpp',
           #hdrs=['move.h'],
           #visibility=['PUBLIC']
          )

cc_library(name='utils',srcs='utils.cpp',
           #hdrs=['utils.h'],
          # visibility=['PUBLIC']
          )
cc_library(name='board',srcs='board.cpp',deps=[':neighbor',':move',':coordinate',':utils'],
           #hdrs=['board.h'],
          # visibility=['PUBLIC']
          )

proto_library(name='train_proto',srcs='train.proto',
             # visibility=['PUBLIC']
             )

cc_library(name='function',srcs='function.cpp',
           #visibility=['PUBLIC']
           deps = [':board'],
          )



#cc_library(name='expert',srcs='expert.cpp',
#           #visibility=['PUBLIC']
#           deps = [':function',':gamestate',':board',':move'],
#          )




proto_library(name='test_proto',srcs='test.proto')


cc_binary(name='proto_test',srcs='proto_test.cpp',deps=[':test_proto'])

#cc_binary(name='write_proto_test',srcs='write_proto_test.cpp',deps=[':gamestate',':train_proto',':utils'])

cc_library(name='gamestate',srcs='gamestate.cpp',deps=[':board',':move',':utils'],
           #hdrs=['gamestate.h'],
          # visibility=['PUBLIC']
          )


cc_binary(name='write_proto_test',srcs='write_proto_test.cpp',deps=[':gamestate',':train_proto',':utils'])


cc_library(name='agent',srcs='agent.cpp',
           #hdrs='agent.h',
           deps=[':gamestate',':board',':coordinate',':utils'],
          # visibility=['PUBLIC']
          )

cc_binary(name='encoder_test',srcs='encoder_test.cpp',deps=[':coordinate',':gamestate',':utils',':agent'],
         # visibility=['PUBLIC']
         )

cc_library(name='expert',srcs='expert.cpp',
           #visibility=['PUBLIC']
           deps = [':function',':gamestate',':board',':move',':coordinate',':utils'],
          )

cc_library(name='alpha_beta',srcs='alpha_beta.cpp',deps=[':gamestate',':expert']),


cc_binary(name='bot_vs_bot_random',srcs='bot_vs_bot_random.cpp',deps=[':neighbor',':coordinate',':gamestate',':utils',':agent'])


cc_binary(name='bot_vs_human',srcs='bot_vs_human.cpp',deps=[':neighbor',':coordinate',':gamestate',':board',':utils',':agent'])

cc_binary(name='legal_moves_test',srcs='legal_moves_test.cpp',deps=[':neighbor',':coordinate',':gamestate',':board',':utils',':move',':agent'])

cc_binary(name='collect_mcts_pure',srcs='bot_vs_bot_mcts_pure.cpp',deps=[':neighbor',':coordinate',':gamestate',':board', ':train_proto', ':utils',':move',':agent','//kingchess_c/net:mcts_pure'])

cc_binary(name='collect_alpha_beta',srcs='collect_alpha_beta.cpp',deps=[':neighbor',':coordinate',':gamestate',':board', ':train_proto', ':utils',':move',':agent',':expert',':alpha_beta'])
cc_binary(name='collect_mcts_pure_add_expert',srcs='bot_vs_bot_mcts_pure_add_expert.cpp',deps=[':neighbor',':coordinate',':gamestate',':board', ':train_proto', ':utils',':move',':expert','//kingchess_c/net:mcts_add_expert'])
