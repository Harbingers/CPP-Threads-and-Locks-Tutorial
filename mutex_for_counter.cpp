// A Thread-Safe Counter with Mutex
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

// a counter, increase 1 whenever increment() is called
struct Counter {
    std::mutex mutex;
    int value;

    Counter() : value(0) {}

    void increment(){
        mutex.lock();
        ++value;
        mutex.unlock();
    }

    void decrement(){
        mutex.lock();
        --value;
        mutex.unlock();
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
 * Compile: clang++ -std=c++11 mutex_for_counter.cpp -pthread
 *
 * Output:
 * 500
 */
