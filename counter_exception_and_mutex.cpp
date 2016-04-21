// A thread-safe counter with mutex and exceptions
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

// a wrapper to take care of the mutex lock
struct ConcurrentCounter {
    std::mutex mutex;
    Counter counter;

    void increment(){
        mutex.lock();
        counter.increment();
        mutex.unlock();
    }

    void decrement(){
        mutex.lock();
        try {
            counter.decrement();
        } catch (std::string e){
            mutex.unlock();
            throw e;
        }
        mutex.unlock();
    }
};

int main(){
    ConcurrentCounter counter;

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

    std::cout << counter.counter.value << std::endl;

    return 0;
}


/*
 * Compile: clang++ -std=c++11 mutex_for_counter.cpp -pthread
 *
 * Output:
 * 500
 */
