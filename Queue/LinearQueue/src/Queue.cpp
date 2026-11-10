#include "Queue.h"

/************************************/

Queue::Queue() = default;

/************************************/

 void Queue::enqueue(int value)
 {
     // Check if the queue is full
     if (isFull())
     {
         // Shift elements to the left
         for (size_t i = 1; i < _count; i++)
         {
             _items[i - 1] = _items[i];
         }

         // Update counters
         --_count;
         ++_droppedCount;
     }

     // Add the new value at the end
     _items[_count] = value;

     ++_count;
 }


bool Queue::peek(int &value) const
{
    if (isEmpty())
    {
        return false;
    }

     value = _items[0];

    return true;
}

 bool Queue::confirmDequeue()
 {
     if (isEmpty())
     {
         return false;
     }

     // Shift elements to the left
     for (size_t i = 0; i < _count - 1; i++)
     {
         _items[i] = _items[i + 1];
     }

     --_count;

     return true;
 }


bool Queue::isFull() const
{
    return _count == QUEUE_CAPACITY;
}

bool Queue::isEmpty() const
{
    return _count == 0;
}

size_t Queue::droppedCount() const
{
    return _droppedCount;
}

size_t Queue::size() const
{
    return _count;
}