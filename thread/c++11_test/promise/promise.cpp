//
//  promise.cpp
//  c++11_test
//
//  Created by Allen on 8/14/16.
//  Copyright © 2016 Vobile. All rights reserved.
//

#include <iostream>       // std::cout
#include <functional>     // std::ref
#include <thread>         // std::thread
#include <future>         // std::promise, std::future

static void print_int(std::future<int>& fut) {
    int x = fut.get(); // 获取共享状态的值.
    std::cout << "value: " << x << '\n'; // 打印 value: 10.
}

int main_promise ()
{
    std::promise<int> prom; // 生成一个 std::promise<int> 对象.
    std::future<int> fut = prom.get_future(); // 和 future 关联.
    std::thread t(print_int, std::ref(fut)); // 将 future 交给另外一个线程t.
   
    try {
        prom.set_value(10);
    }
    catch (std::future_error& e) {
        if (e.code() == std::make_error_condition(std::future_errc::promise_already_satisfied)) {
            std::cerr << "[promise_already_satisfied]\n";
            throw;
        }
        else std::cerr << "[unknown exception]\n";
    }
    
    t.join();
    return 0;
}
