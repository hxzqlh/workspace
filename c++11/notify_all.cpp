#include <thread>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <random>

std::mutex              g_lockprint;
std::mutex              g_lock;
std::condition_variable g_signal;
bool                    g_done;
 
void workerfunc(std::mt19937& generator)
{
   {
      std::unique_lock<std::mutex> locker(g_lockprint);
      std::cout << "worker running..." << std::endl;
   }
 
   std::this_thread::sleep_for(std::chrono::seconds(1 + generator() % 5));
 
   {
      std::unique_lock<std::mutex> locker(g_lockprint);
      std::cout << "worker finished..." << std::endl;
   }
 
   std::unique_lock<std::mutex> lock(g_lock);
   g_done = true;
   std::notify_all_at_thread_exit(g_signal, std::move(lock));
}
 
int main()
{
   // initialize a random generator
   std::mt19937 generator((unsigned int)std::chrono::system_clock::now().time_since_epoch().count());
 
   std::cout << "main running..." << std::endl;
 
   std::thread worker(workerfunc, std::ref(generator));
   worker.detach();
 
   std::cout << "main crunching..." << std::endl;
 
   std::this_thread::sleep_for(std::chrono::seconds(1 + generator() % 5));
 
   {
      std::unique_lock<std::mutex> locker(g_lockprint);
      std::cout << "main waiting for worker..." << std::endl;
   }
 
   std::unique_lock<std::mutex> lock(g_lock);
   while(!g_done) // avoid spurious wake-ups
      g_signal.wait(lock);
 
   std::cout << "main finished..." << std::endl;
 
   return 0;
}
