
// A thread-safe counter with mutex and exceptions and auto-release locks (lock_guard)
// Reference: http://baptiste-wicht.com/posts/2012/03/cp11-concurrency-tutorial-part-2-protect-shared-data.html

/*
 * 1. Problem Without Locks:
 * incrementation is not an atomic operation. It is made of three operations:
 * - Read the current value of value
 * - Add one to the current value
 * - Write that new value to value
 *
 * 2. Use a mutex to make counter thread-safe
 * 3. Consider exception: need to unlock() in a try-catch block, otherwise, will never unlock()
 * 4. If we want to: use std::lock_guard(), when the guard gets destructed, it releases the lock
 *    1) protect a whole block of code
 *    2) may forget to release the lock
*/

#include <thread>
#include <iostream>
#include <vector>
#include <mutex>

// a counter,
struct Counter {
    int value;

    Counter() : value(0) {}

    // increase 1 whenever increment() is called
    void increment(){
        ++value;
    }
    // decrease 1 whenever decrement() is called
    // throw an exception if the value == 0
    void decrement(){
        if(value == 0){
            throw "Value cannot be less than 0";
        }
        --value;
    }
};

// a wrapper to take care of the mutex lock, and release the lock automatically
struct ConcurrentSafeCounter {
    std::mutex mutex;
    Counter counter;

    void increment(){
        std::lock_guard<std::mutex> guard(mutex);
        counter.increment();
    }

    void decrement(){
        std::lock_guard<std::mutex> guar(mutex);
        counter.decrement();
    }
};

int main(){
    Counter counter;

    std::vector<std::thread> threads;

    for(int i = 0; i < 5; ++i){
        threads.push_back(std::thread([&](){
            for(int i = 0; i < 100; ++i){
                counter.increment();
            }
        }));
    }

    for(auto& thread : threads){
        thread.join();
    }

    std::cout << counter.value << std::endl;

    return 0;
}

/*
 * Compile: clang++ -std=c++11 auto_release_lock_lockguard_for_counter.cpp -pthread
 *
 * Output:
 * 500
 */
