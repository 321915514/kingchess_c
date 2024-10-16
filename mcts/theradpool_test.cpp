//
// Created by wangxu on 2023/11/1.
//
#include "threadpool.h"

int main() {
    ThreadPool pool(4);

    std::vector<std::future<void>> results;

    for (int i = 0; i < 8; ++i) {
        results.emplace_back(pool.enqueue([]{
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }));
    }

    for (auto&& result : results) {
        result.get();
    }

    std::cout<<"hello"<<std::endl;

    return 0;
}