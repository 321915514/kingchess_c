//
// Created by wangxu on 2023/10/24.
//

#ifndef LONGCHESS_ENGINE_PARAM_H
#define LONGCHESS_ENGINE_PARAM_H
class EngineParam{
public:
    // Search
    bool m_is_sync = false;
    bool m_thinking_always = false;
    int m_thread_num = 1;
    double m_virtual_count = 4;
    double m_policy_weight = 1.1;
    double m_policy_multi_pow = 0.35;
    double m_policy_multi_point = 1000;
    double m_vloss = 0.3;
    double m_max_vloss_multiplier = 1.0;
    int m_back_prop_area = 1;
    int m_evaluating = 0;
    double m_default_winrate_k1 = 0.6;
    double m_default_winrate_k2 = 0.1;
    double m_default_winrate_k3 = 2.0;
    double m_default_winrate_k4 = 0.0;
    double m_default_winrate_k5 = 0.0;
    double m_pop_node_policy_thres = 0.0;
    int m_max_playout = 100000000;
    double m_draw_game_value_pow = 0.5;
    double m_discard_winrate_thres = 0.0;
    double m_next_add_policy = 0.1;

    // Root
    bool m_use_root_strategy = false;
    double m_root_policy_weight = 1.1;
    double m_root_policy_temperature = 1.1;
    double m_root_policy_temperature_2 = 1.1;
    double m_root_policy_temperature_p = 1.1;

    // Genmove
    bool m_genmove_by_origin = true;
    bool m_genmove_by_early = false;
    bool m_genmove_by_lcb = false;
    bool m_genmove_by_kl = false;
//    GradualParam m_genmove_temperature;
//    GradualParam m_genmove_proportion_thres;
//    GradualParam m_genmove_gap_multi;
//    GradualParam m_genmove_delta_winrate_thres;
    int m_policy_genmove_num = 0;
    double m_policy_genmove_temperature = 1.0;
    double m_meaningful_node_thres = 0.05;
    int m_genmove_playout = 1600;

    // Noise
    double m_dirichelt_policy_thres = 0.00005;
    double m_dirichelt_policy_winner_min = 0.1;
    double m_dirichelt_policy_loser_min = 0.1;
//    GradualParam m_dirichelt_noise_policy;
//    GradualParam m_dirichelt_noise_value;

    // lambda
//    GradualParam m_winrate_lambda;
//    GradualParam m_delta_lambda;
//    GradualParam m_delta_pow;
//    GradualParam m_value_lambda;
//    GradualParam m_area_lambda;
//    GradualParam m_score_lambda;
//    GradualParam m_delta_area_lambda;

    // Display
    bool m_log_tree_shape = false;
    bool m_log_node_info = false;
    bool m_log_options = true;
    bool m_log_graph = true;
    bool m_log_winrate = true;
    bool m_log_do_move_situation = true;
    bool m_log_spotlight = false;
    bool m_log_expanded_node_num = true;
    bool m_log_performance = false;
    bool m_log_do_move = true;
    bool m_log_genmove_quantum = true;
    bool m_log_play_second_child = true;
    double m_node_info_thres = 0.01;
    double m_node_info_proportion_thres = 0.2;
    double m_options_thres = 0.001;
    bool m_log_debug = false;

    // Resign
//    bool m_resign_by_area = false;
//    bool m_resign_by_finish = false;
//    GradualParam m_resign_value;
//    GradualParam m_resign_delta;

    // Clock
    int m_base_time_in_minutes = 0;
    double m_counting_time_in_seconds = 5;
    int m_counting_times = 1;
    double m_reserved_time_in_seconds = 30;
    double m_clock_accelerate = 0;
    double m_clock_miu_0 = 0.3;
    double m_clock_sigma_0 = 10;
    double m_clock_mouse_time = 3;
    double m_clock_deliberate = 100000;
    double m_clock_prob_thres_0 = 0.01;
    double m_clock_prob_thres_1 = 0.25;
    double m_winner_speed_up_max = 1.0;
    double m_winner_speed_up_start = 0.95;
    double m_search_speed = 10000;
    bool m_log_clock_detail = false;
    double m_max_move_time_in_seconds = 60;
    double m_min_move_time_in_seconds = 1;
    bool m_use_multiple_counting = false;
    bool m_thinking_till_counting_end = false;
    double m_pretend_think_time = 0;
    double m_opening_book_min_think_time = 0;
    double m_opening_book_max_think_time = 0;;

    // Restore
    bool m_use_restore = false;
    double m_restore_sigma_pow_1 = 0.25;
    double m_restore_sigma_pow_2 = 0.25;
    double m_restore_prior_num = 16;
    double m_restore_max_multi = 6.0;
    double m_restore_min_multi = 4.0;

    // Balance
    int m_balance_frequency = 0;
    bool m_balance_urgency = false;
    bool m_log_balance = false;
    double m_balance_gap = 0.0;
    double m_balance_var = 0.0;
    double m_balance_thres = 1.0;

    // DogDetector
    bool m_use_dog_detector = false;
    int m_dog_observe_move_num = 16;
    int m_dog_winrate_observe_move_num = 6;
    double m_dog_winrate_thres_1 = 0.8;
    double m_dog_winrate_thres_2 = 0.9;
    double m_dog_proportion_thres_1 = 0.92;
    double m_dog_proportion_thres_2 = 0.96;
    double m_dog_average_time_thres = 2;

    // Others
    bool m_imitation_strategy = true;
    double m_spotlight_move_decay = 0.1;
    bool m_reply_show_proportion = false;

};
#endif //LONGCHESS_ENGINE_PARAM_H
