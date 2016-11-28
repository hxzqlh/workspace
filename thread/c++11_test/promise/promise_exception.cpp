//
//  promise_exception.cpp
//  c++11_test
//
//  Created by Allen on 8/14/16.
//  Copyright © 2016 Vobile. All rights reserved.
//

#include <iostream>       // std::cin, std::cout, std::ios
#include <functional>     // std::ref
#include <thread>         // std::thread
#include <future>         // std::promise, std::future
#include <exception>      // std::exception, std::current_exception

void get_int(std::promise<int>& prom) {
    int x;
    std::cout << "Please, enter an integer value > 3: ";
    std::cin >> x;
    if (x > 3)
        prom.set_value(x);
    else {
        prom.set_value(x);
        auto p = std::make_exception_ptr(std::logic_error("value not > 3"));
        prom.set_exception(p);
    }
}

void print_int(std::future<int>& fut) {
    try {
        int x = fut.get();
        std::cout << "value: " << x << '\n';
    } catch (std::exception& e) {
        std::cout << "[exception caught: " << e.what() << "]\n";
    }
}

int main_exception ()
{
    std::promise<int> prom;
    std::future<int> fut = prom.get_future();
    
    std::thread th1(get_int, std::ref(prom));
    std::thread th2(print_int, std::ref(fut));
    
    th1.join();
    th2.join();
    return 0;
}
