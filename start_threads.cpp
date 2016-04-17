// Starting a new thread
// Reference: http://baptiste-wicht.com/posts/2012/03/cpp11-concurrency-part1-start-threads.html

#include <thread>
#include <iostream>

void hello(){
    std::cout << "Hello from thread " << std::endl;
}

int main(){
    std::thread t(hello);
    // Blocks the current thread until the thread finishes its execution.
    // When it is done: joinable is false
    t.join();
    return 0;
}

/*
Output:
Hello from thread 
Program ended with exit code: 0
*/
