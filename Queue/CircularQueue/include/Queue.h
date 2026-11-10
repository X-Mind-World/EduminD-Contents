#pragma once

#include <Arduino.h>

class Queue
{
public:
    static constexpr size_t QUEUE_CAPACITY = 4;

    Queue();

    void enqueue(int value);
    bool peek(int &value) const;
    bool confirmDequeue();

    bool isFull() const;
    bool isEmpty() const;

    size_t size() const;
    size_t droppedCount() const;

private:
    int _items[QUEUE_CAPACITY]{};

//  --For Ring Buffer-- 
    size_t _front = 0;
    size_t _rear = 0;
//------------------- 

    size_t _count = 0;
    size_t _droppedCount = 0;
};