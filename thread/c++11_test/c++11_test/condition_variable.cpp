//
//  condition_variable.cpp
//  c++11_test
//
//  Created by Allen on 6/26/16.
//  Copyright © 2016 Vobile. All rights reserved.
//


// condition_variable example
#include <iostream>           // std::cout
#include <thread>             // std::thread
#include <mutex>              // std::mutex, std::unique_lock
#include <condition_variable> // std::condition_variable

static std::mutex mtx;
static std::condition_variable cv;
static bool ready = false;

static void print_id (int id) {
    std::unique_lock<std::mutex> lck(mtx);
    while (!ready) {
        cv.wait(lck);
    }
    
    std::cout << "id=" << id << std::endl;
}

static void go () {
    std::unique_lock<std::mutex> lck(mtx);
    ready = true;
    cv.notify_all();
}

int main_wait () {
    std::thread threads[10];
    // spawn 10 threads:
    for (int i=0; i<10; ++i)
        threads[i] = std::thread(print_id,i);
    
    std::cout << "10 threads ready to race...\n";
    go();                       // go!
    
    for (auto& th : threads)
        th.join();
    
    return 0;
}
