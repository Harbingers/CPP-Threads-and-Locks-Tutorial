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

std::timed_mutex timed_mutex;

void work(){
    std::chrono::milliseconds timeout(1000);

    while(true){
        if(timed_mutex.try_lock_for(timeout)){
            std::cout << std::this_thread::get_id() << ": with the mutex" << std::endl;
            std::chrono::milliseconds sleepDuration(3000);
            std::this_thread::sleep_for(sleepDuration);
            timed_mutex.unlock();
        } else {
            std::cout << std::this_thread::get_id() << ": without mutex" << std::endl;
            std::chrono::milliseconds sleepDuration(1000);
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
3074476864: with the mutex
3066084160: without mutex
3066084160: without mutex
3066084160: without mutex
3074476864: with the mutex
3066084160: without mutex
3066084160: without mutex
3066084160: without mutex
3074476864: with the mutex
3066084160: without mutex
3066084160: without mutex
3066084160: without mutex
3074476864: with the mutex
3066084160: without mutex
...
*/
