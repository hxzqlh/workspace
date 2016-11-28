//
//  unique_lock.cpp
//  c++11_test
//
//  Created by Allen on 6/26/16.
//  Copyright © 2016 Vobile. All rights reserved.
//

#include <iostream>       // std::cout
#include <thread>         // std::thread
#include <mutex>          // std::mutex, std::unique_lock, std::defer_lock

class MyMutex : public std::mutex {
    int _id;
public:
    MyMutex (int id) : _id(id) {}
    int id() {return _id;}
};

static MyMutex mtx (101);

static void print_ids (int id) {
    std::unique_lock<MyMutex> lck (mtx);
    std::cout << "thread #" << id << " locked mutex " << lck.mutex()->id() << '\n';
}

int main_unique_lock ()
{
    std::thread threads[10];    
    // spawn 10 threads:
    for (int i=0; i<10; ++i)
        threads[i] = std::thread(print_ids,i+1);
    
    for (auto& th : threads) th.join();
    
    return 0;
}