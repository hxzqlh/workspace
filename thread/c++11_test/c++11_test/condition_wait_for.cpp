//
//  condition_wait_for.cpp
//  c++11_test
//
//  Created by Allen on 6/26/16.
//  Copyright © 2016 Vobile. All rights reserved.
//

#include <iostream>           // std::cout
#include <thread>             // std::thread
#include <chrono>             // std::chrono::seconds
#include <mutex>              // std::mutex, std::unique_lock
#include <condition_variable> // std::condition_variable, std::cv_status

static std::condition_variable cv;

static int value;

void do_read_value()
{
    std::cin >> value;
    cv.notify_one();
}

int main3 ()
{
    using namespace std::chrono;
    std::cout << "Please, enter an integer (I'll be printing dots): \n";
    std::thread th(do_read_value);
    
    std::mutex mtx;
    std::unique_lock<std::mutex> lck(mtx);
    
    while (cv.wait_until(lck,system_clock::now() + seconds(1)) == std::cv_status::timeout) {
        std::cout << '.';
        std::cout.flush();
    }
    
    std::cout << "You entered: " << value << '\n';
    
    th.join();
    return 0;
}