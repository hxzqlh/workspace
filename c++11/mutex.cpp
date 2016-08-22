#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

using namespace std;

mutex g_lock;

void func ()
{
    g_lock.lock ();
    cout << "entered thread " << this_thread::get_id () << endl;
    this_thread::sleep_for (chrono::seconds (rand() % 10));
    cout << "leaving thread " << this_thread::get_id () << endl;
    g_lock.unlock ();
}

int main ()
{
    srand ((unsigned int) time(0));

    thread t1(func);
    thread t2(func);
    thread t3(func);

    t1.join();
    t2.join();
    t3.join();
    
    return 0;
}
