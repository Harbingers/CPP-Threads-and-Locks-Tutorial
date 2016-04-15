// Starting a new thread
// Reference: http://baptiste-wicht.com/posts/2012/03/cpp11-concurrency-part1-start-threads.html

#include <thread>
#include <iostream>

void hello(){
    std::cout << "Hello from thread " << std::endl;
}

int main(){
    std::thread t1(hello);
    t1.join();

    return 0;
}
