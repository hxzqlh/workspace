//
//  packaged_task.cpp
//  c++11_test
//
//  Created by Allen on 8/27/16.
//  Copyright © 2016 Vobile. All rights reserved.
//

#include <stdio.h>
#include <future>
#include <iostream>
#include <thread>

int count_down(int from, int to)
{
    for (int i=from;i!=to;--i) {
        std::cout << i << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    
    std::cout << "Finished!\n";
    return from - to;
}

int main_packaged_task ()
{
    std::packaged_task<int(int,int)> task(count_down);
    std::future<int> fut = task.get_future();
    
    std::thread th(std::move(task), 10, 0);
    
    int ret = fut.get();
    
    std::cout << "The countdown lasted for " << ret << " seconds.\n";
    
    th.join();
    
    return 0;
}
