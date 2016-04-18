// A Thread-Safe Counter 
// Reference: http://baptiste-wicht.com/posts/2012/03/cp11-concurrency-tutorial-part-2-protect-shared-data.html

struct Counter {
    int value;
    Counter() : value(0){}
    void increment(){
        ++value;
    }
};
