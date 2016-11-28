//
//  lock_guard.cpp
//  c++11_test
//
//  Created by Allen on 6/26/16.
//  Copyright © 2016 Vobile. All rights reserved.
//

// lock_guard example
#include <iostream>       // std::cout
#include <thread>         // std::thread
#include <mutex>          // std::mutex, std::lock_guard
#include <stdexcept>      // std::logic_error

static std::mutex mtx;

void print_even (int x) {
    if (x%2==0)
        std::cout << x << " is even\n";
    else
        throw (std::logic_error("not even"));
}

void print_id (int id) {
    try {
        // using a local lock_guard to lock mtx guarantees unlocking on destruction / exception:
        std::lock_guard<std::mutex> lck (mtx);
        print_even(id);
    }
    catch (std::logic_error&) {
        std::cout << "[exception caught]\n";
    }
}

int main_guard ()
{
    std::thread threads[10];
    // spawn 10 threads:
    for (int i=0; i<10; ++i)
        threads[i] = std::thread(print_id,i+1);
    
    for (auto& th : threads)
        th.join();
    
    return 0;
}
