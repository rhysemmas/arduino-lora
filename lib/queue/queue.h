#include <packet.h>

// Define the default capacity of a queue
#define SIZE 10

class queue {
    Packet *arr;    // array to store queue elements
    int capacity;   // maximum capacity of the queue
    int front;      // front points to the front element in the queue (if any)
    int rear;       // rear points to the last element in the queue
    int count;      // current size of the queue

public:
    queue(int size = SIZE);  // constructor
    ~queue();                // destructor

    bool dequeue();          // return true for successful dequeue, else false
    bool enqueue(Packet x);  // return true for successful enqueue, else false
    Packet peek();
    int size();
    bool isEmpty();
    bool isFull();
};
