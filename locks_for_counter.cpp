// A Thread-Safe Counter 
// Reference: http://baptiste-wicht.com/posts/2012/03/cp11-concurrency-tutorial-part-2-protect-shared-data.html

#include <thread>
#include <iostream>
#include <vector>

// a counter, increase 1 whenever increment() is called
struct Counter {
    int value;
    Counter() : value(0){}
    void increment(){
        ++value;
    }
};

int main(){
    Counter counter;

    std::vector<std::thread> threads;
    for(int i = 0; i < 5; ++i){
        threads.push_back(std::thread([&counter](){
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

/**
 * Output:
 * 500
 * /
