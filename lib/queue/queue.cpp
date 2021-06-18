#include "queue.h"

// Constructor to initialize a queue
queue::queue(int size) {
    arr = new Packet[size];
    capacity = size;
    front = 0;
    rear = -1;
    count = 0;
}

// Destructor to free memory allocated to the queue
queue::~queue() {
    delete[] arr;
}

// Utility function to dequeue the front element
bool queue::dequeue() {
    // check for queue underflow
    if (isEmpty())
    {
      return false;
    }

    front = (front + 1) % capacity;
    count--;
    return true;
}

// Utility function to add an item to the queue
bool queue::enqueue(Packet item) {
    // check for queue overflow
    if (isFull())
    {
      return false;
    }

    rear = (rear + 1) % capacity;
    arr[rear] = item;
    count++;
    return true;
}

// Utility function to return the front element of the queue
Packet queue::peek() {
    if (isEmpty())
    {
      return Packet{};
    }
    return arr[front];
}

// Utility function to return the size of the queue
int queue::size() {
    return count;
}

// Utility function to check if the queue is empty or not
bool queue::isEmpty() {
    return (size() == 0);
}

// Utility function to check if the queue is full or not
bool queue::isFull() {
    return (size() == capacity);
}
