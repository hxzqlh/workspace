//
//  future_share.cpp
//  c++11_test
//
//  Created by Allen on 8/27/16.
//  Copyright © 2016 Vobile. All rights reserved.
//

#include <iostream>       // std::cout
#include <future>         // std::async, std::future, std::shared_future

int do_get_value() { return 10; }

int main ()
{
    std::future<int> fut = std::async(do_get_value);
    std::shared_future<int> shared_fut = fut.share();
    
    std::cout << "valid:" << fut.valid() << std::endl;
    std::cout << "valid:" << shared_fut.valid() << std::endl;
    
    // 共享的 future 对象可以被多次访问.
    std::cout << "value: " << shared_fut.get() << '\n';
    std::cout << "its double: " << shared_fut.get()*2 << '\n';
    
    
    
    return 0;
}