// Create 5 threads, print out their thread ids
// Reference: http://baptiste-wicht.com/posts/2012/03/cpp11-concurrency-part1-start-threads.html

#include <thread>
#include <iostream>
#include <vector>

void hello(){
    // std::this_thread is current thread
    std::cout << "Hello from thread " << std::endl << std::this_thread::get_id() << std::endl;
}

int main(){
    std::vector<std::thread> threads;
    
    for(int i = 0; i < 5; ++i){
        threads.push_back(std::thread(hello));
    }
    
    for(auto& thread : threads){
        thread.join();
    }
    
    return 0;
}

/* 
Output:
HHHHHeeeeellllllllllooooo     fffffrrrrrooooommmmm     ttttthhhhhrrrrreeeeeaaaaaddddd     




00000xxxxx111110000000000234348800817a4d000000000000000




Program ended with exit code: 0

*/
