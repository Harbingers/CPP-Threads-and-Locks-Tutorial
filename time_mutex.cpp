// A time mutex


// Reference: http://baptiste-wicht.com/posts/2012/04/c11-concurrency-tutorial-advanced-locking-and-condition-variables.html
//=======================================================================
// Copyright (c) 2014 Baptiste Wicht
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

// std::timed_mutex: try_lock_for() -- will work if gets the lock or if timeout

#include <thread>
#include <mutex>
#include <chrono>
#include <iostream>

std::timed_mutex mutex;

void work(){
    std::chrono::milliseconds timeout(100);

    while(true){
        if(mutex.try_lock_for(timeout)){
            std::cout << std::this_thread::get_id() << ": do work with the mutex" << std::endl;
            std::chrono::milliseconds sleepDuration(250);
            std::this_thread::sleep_for(sleepDuration);
            mutex.unlock();
        } else {
            std::cout << std::this_thread::get_id() << ": do work without mutex" << std::endl;
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
Output:
3065817920: without mutex
3074210624: with the mutex
3065817920: without mutex
3065817920: without mutex
3074210624: with the mutex
3065817920: without mutex
3065817920: without mutex
3074210624: with the mutex
3065817920: without mutex
3065817920: without mutex
3065817920: without mutex
3074210624: with the mutex
3065817920: without mutex
3065817920: without mutex
3074210624: with the mutex
3065817920: without mutex
3065817920: without mutex
3065817920: without mutex


...
*/


