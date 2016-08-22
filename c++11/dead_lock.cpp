#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <set>

using namespace std;

template <typename T>
class container 
{
public:
    std::mutex _lock;
    std::set<T> _elements;

    void add(T element) 
    {
        _elements.insert(element);
    }

    void remove(T element) 
    {
        _elements.erase(element);
    }
};

void exchange(container<int>& cont1, container<int>& cont2, int value)
{
    lock (cont1._lock, cont2._lock);

    cont1.remove(value);
    cont2.add(value);

    cont1._lock.unlock();
    cont2._lock.unlock();
}

int main()
{
    srand((unsigned int)time(NULL));
 
    container<int> cont1; 
    cont1.add(1);
    cont1.add(2);
    cont1.add(3);
 
    container<int> cont2; 
    cont2.add(4);
    cont2.add(5);
    cont2.add(6);
 
    std::thread t1(exchange, std::ref(cont1), std::ref(cont2), 3);
    std::thread t2(exchange, std::ref(cont2), std::ref(cont1), 6);
 
    t1.join();
    t2.join();
 
    return 0;
}

