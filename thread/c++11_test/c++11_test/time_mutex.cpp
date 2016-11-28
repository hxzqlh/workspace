//
//  time_mutex.cpp
//  c++11_test
//
//  Created by Allen on 6/25/16.
//  Copyright © 2016 Vobile. All rights reserved.
//

#include <stdio.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>
using namespace std;

static timed_mutex tmtx;

void fire_works ()
{
    while (!tmtx.try_lock_for(chrono::milliseconds(200))){
        cout << "-";
    }
    
    this_thread::sleep_for(chrono::milliseconds(500));
    cout << "+";
    tmtx.unlock();
}

int main_timed_mutex ()
{
    thread ths[10];
    
    for (int i=0; i<10; ++i) {
        ths[i] = thread(fire_works);
    }
    
    for (auto &th : ths ) {
        th.join();
    }
    
    return 0;
}