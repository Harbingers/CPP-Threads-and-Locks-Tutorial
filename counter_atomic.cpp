// An Atomic Counter
// Reference: http://baptiste-wicht.com/posts/2012/07/c11-concurrency-tutorial-part-4-atomic-type.html

//=======================================================================
// Copyright (c) 2014 Baptiste Wicht
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

/*
 1. std::atomic, you can use any type with that template and the ops on that variable, it will be atomic (thread safe)

 2. It is up to the lib implementation to choose which synchronization is used to make atomic.
    iF it is int. long. float, it will be lock-free technique, good
    if it is big type, mutexes will be used, no performance advantage
 */

#include <thread>
#include <atomic>
#include <iostream>
#include <vector>

struct AtomicCounter {
    std::atomic<int> value;

    AtomicCounter() : value(0) {}

    void increment(){
        ++value;
    }

    void decrement(){
        --value;
    }

    int get(){
        return value.load();
    }
};

int main(){
    AtomicCounter counter;

    std::vector<std::thread> threads;
    for(int i = 0; i < 10; ++i){
        threads.push_back(std::thread([&counter](){
            for(int i = 0; i < 500; ++i){
                counter.increment();
            }
        }));
    }

    for(auto& thread : threads){
        thread.join();
    }

    std::cout << counter.get() << std::endl;

    return 0;
}

/*
Compile: clang++ -std=c++11 counter_atomic.cpp -pthread

Output: 5000

*/
