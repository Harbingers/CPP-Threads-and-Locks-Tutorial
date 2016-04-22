// A function that will only be called once
// Reference: http://baptiste-wicht.com/posts/2012/04/c11-concurrency-tutorial-advanced-locking-and-condition-variables.html

//=======================================================================
// Copyright (c) 2014 Baptiste Wicht
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

// std::call_once - If want one function to be called only once


#include <thread>
#include <chrono>
#include <iostream>
#include <mutex>

std::once_flag once_flag;

void do_something(){
    std::call_once(once_flag, [](){std::cout << std::this_thread::get_id() << " Called once" << std::endl;});
    std::cout << std::this_thread::get_id() << " Called each time" << std::endl;
}

int main(){
    std::thread t1(do_something);
    std::thread t2(do_something);
    std::thread t3(do_something);
    std::thread t4(do_something);

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    return 0;
}

/*
Compile: clang++ -std=c++11 called_once_function.cpp -pthread

Output:
3074575168 Called once
3074575168 Called each time
3066182464 Called each time
3057789760 Called each time
3049397056 Called each time

*/
