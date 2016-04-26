// A Concurrent Bounded Buffer
// Reference: http://baptiste-wicht.com/posts/2012/04/c11-concurrency-tutorial-advanced-locking-and-condition-variables.html

//=======================================================================
// Copyright (c) 2014 Baptiste Wicht
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================



/*
1. C++11 标准为我们提供了两种基本的锁类型，分别如下：
   1) std::lock_guard，与 Mutex RAII 相关，方便线程对互斥量上锁。简单版。
   2) std::unique_lock，与Mutex RAII 相关，方便线程对互斥量上锁。复杂版。
      - 以独占所有权的方式, 管理 mutex 对象的上锁和解锁操作
      - 没有其他的 unique_lock 对象同时拥有某个 mutex 对象的所有权。
      - unique_lock 对象只是简化了 Mutex 对象的上锁和解锁操作，方便线程对互斥量上锁，
        即在某个 unique_lock 对象的声明周期内，它所管理的锁对象会一直保持上锁状态；
        而 unique_lock 的生命周期结束之后，它所管理的锁对象会被解锁.
      - unique_lock 对象同样也不负责管理 Mutex 对象的生命周期


2. A condition variable manages a list of threads waiting until another thread notify them

3. std::lock——可以一次性锁住多个(两个以上)的互斥量，并且没有副作用(死锁风险)。

*/
#include <thread>
#include <mutex>
#include <chrono>
#include <iostream>
#include <condition_variable>

struct BoundedBuffer {

    int* buffer;
    int capacity;

    int front;
    int rear;
    int count;

    std::mutex mutex;

    std::condition_variable not_full;
    std::condition_variable not_empty;

    BoundedBuffer(int capacity) : capacity(capacity), front(0), rear(0), count(0) {
        buffer = new int[capacity];
    }

    ~BoundedBuffer(){
        delete[] buffer;
    }

    void put(int data){

        std::unique_lock<std::mutex> l(mutex);

        not_full.wait(l, [this](){return count != capacity; });

        buffer[rear] = data;
        rear = (rear + 1) % capacity;
        ++count;

        not_empty.notify_one();
    }

    int get(){
        std::unique_lock<std::mutex> l(mutex);

        not_empty.wait(l, [this](){return count != 0; });

        int result = buffer[front];
        front = (front + 1) % capacity;
        --count;

        not_full.notify_one();

        return result;
    }
};

// Several threads (consumers) are waiting from data produced by another several threads (producers).
void consumer(int id, BoundedBuffer& buffer){
    for(int i = 0; i < 50; ++i){
        int value = buffer.get();
        std::cout << "Consumer " << id << " fetched " << value << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
    }
}

void producer(int id, BoundedBuffer& buffer){
    for(int i = 0; i < 75; ++i){
        buffer.put(i);
        std::cout << "Produced " << id << " produced " << i << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

int main(){
    BoundedBuffer buffer(200);
    // std::ref to pass the buffer by reference, it is necessary to avoid a copy of the buffer.
    std::thread c1(consumer, 0, std::ref(buffer));
    std::thread c2(consumer, 1, std::ref(buffer));
    std::thread c3(consumer, 2, std::ref(buffer));
    std::thread p1(producer, 0, std::ref(buffer));
    std::thread p2(producer, 1, std::ref(buffer));

    c1.join();
    c2.join();
    c3.join();
    p1.join();
    p2.join();

    return 0;
}

/*
 Compile: clang++ -std=c++11 bounded_buffer_condition_variable.cpp -pthread

 Output:
 Produced 0 produced 0
Consumer 0 fetched 0
Consumer 1 fetched 0
Produced 1 produced 0
Produced 0 produced 1
Consumer 2 fetched 1
Produced 1 produced 1
Produced 0 produced 2
Produced 1 produced 2
Consumer 1 fetched 1
Consumer 0 fetched 2
Produced 0 produced 3
Produced 1 produced 3
Consumer 2 fetched 2
Produced 0 produced 4Produced
1 produced 4
Consumer 1 fetched 3
Consumer 0 fetched 3
Produced 0 produced Produced 5
1 produced 5
Consumer 2 fetched 4
Produced 0 produced 6
Produced 1 produced 6
Produced 0 produced 7
Produced 1 produced 7
Consumer 1 fetched 4
Consumer 0 fetched 5
Produced 1 produced 8
Produced 0 produced 8
Consumer 2 fetched 5
Produced 0 produced 9Produced
1 produced 9
Consumer 1 fetched 6
Consumer 0 fetched 6
Produced 0 produced 10
Produced 1 produced 10
Consumer 2 fetched 7
Produced 0 produced 11
Produced 1 produced 11
Produced 0 produced 12
Produced 1 produced 12
Consumer 1 fetched 7
Consumer 0 fetched 8



*/
