// A time mutex


// Reference: http://baptiste-wicht.com/posts/2012/04/c11-concurrency-tutorial-advanced-locking-and-condition-variables.html
//=======================================================================
// Copyright (c) 2014 Baptiste Wicht
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================


#include <thread>
#include <mutex>
#include <chrono>
#include <iostream>

std::timed_mutex timed_mutex;

void work(){
    std::chrono::milliseconds timeout(100);

    while(true){
        if(timed_mutex.try_lock_for(timeout)){ // 等待锁, 一定时限内等到了, 返回true; 没等到, 返回false
            std::cout << std::this_thread::get_id() << ": with the mutex" << std::endl;

            std::chrono::milliseconds sleepDuration(250);
            std::this_thread::sleep_for(sleepDuration);

            timed_mutex.unlock();

            std::this_thread::sleep_for(sleepDuration);
        } else {
            std::cout << std::this_thread::get_id() << ": without mutex" << std::endl;

            std::chrono::milliseconds sleepDuration(100);
            std::this_thread::sleep_for(sleepDuration);
        }
    }
}

int main(){
    std::thread t1(work);
    std::thread t2(work);

    t1.join();
    t2.join();

    return 0;
}

/*
Compile: clang++ -std=c++11 time_mutex.cpp -pthread


Output:
3065948992: with the mutex
3074341696: without mutex
3074341696: without mutex
3074341696: without mutex
3074341696: with the mutex
3065948992: without mutex
3065948992: with the mutex
3074341696: without mutex
3074341696: with the mutex
3065948992: without mutex
3065948992: with the mutex
3074341696: without mutex
3074341696: with the mutex
3065948992: without mutex
3065948992: with the mutex
3074341696: without mutex
3074341696: with the mutex
3065948992: without mutex
3065948992: with the mutex
3074341696: without mutex
3074341696: with the mutex
3065948992: without mutex
3065948992: with the mutex
3074341696: without mutex
3074341696: with the mutex
3065948992: without mutex
3065948992: with the mutex

...
*/
