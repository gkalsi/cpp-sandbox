// Adapted from the example at this page:
// https://en.cppreference.com/w/cpp/thread/condition_variable

// The timeline of events is as follows:
// 
//                   T1 timeout          T2 timeout          T3 timeout
// 0                    5                   10                   15
// |--------------------|--------------------|--------------------|
//                      ^    ^               ^    ^
//                      |    |               |    |
//  T1 times out and    +    |               |    |
//  checks i == 1?           |               |    |
//                           |               |    |
//  All remaining threads    |               |    |
//  are signalled and check -+               |    |
//  i == 1?                                  |    |
//                                           |    |
//  T2 times out and checks i == 1? ---------+    |
//                                                |
//  i is set to 1 and all threads and signalled --+
//                                                |
//  T3 wakes and checks i == 1, which is now -----+
//  true. T3 finishes waiting.


#include <iostream>
#include <atomic>
#include <condition_variable>
#include <thread>
#include <chrono>

using namespace std::chrono_literals;
 
constexpr int kResolution = 500;

std::condition_variable cv;
std::mutex cv_m;
int i;
 
void waits(int idx) {
    std::unique_lock<std::mutex> lk(cv_m);
    if(cv.wait_for(lk, idx * kResolution * 5ms, [idx]{
      std::cout << idx << " wake and check" << std::endl;
      return i == 1;
    })) {
        std::cout << "Thread " << idx << " finished waiting. i == " << i << '\n';
    } else {
        std::cout << "Thread " << idx << " timed out. i == " << i << '\n';
    }
}
 
void signals() {
    std::this_thread::sleep_for(kResolution * 6ms);
    std::cout << "Notifying...\n";
    cv.notify_all();
    std::this_thread::sleep_for(kResolution * 5ms);
    {
        std::lock_guard<std::mutex> lk(cv_m);
        i = 1;
    }
    std::cout << "Notifying again...\n";
    cv.notify_all();
}
 
int main()
{
    std::thread t1(waits, 1), t2(waits, 2), t3(waits, 3), t4(signals);
    t1.join();
    t2.join();
    t3.join();
    t4.join();
}