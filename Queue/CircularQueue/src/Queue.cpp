#include "Queue.h"

/************************************/

Queue::Queue() = default;

/************************************/

void Queue::enqueue(int value)
{
    if (isFull())
    {
        ++_droppedCount;

        _front = (_front + 1) % QUEUE_CAPACITY;
    }
    else
    {
        ++_count;
    }

    _items[_rear] = value;

    _rear = (_rear + 1) % QUEUE_CAPACITY;
}

bool Queue::peek(int &value) const
{
    if (isEmpty())
    {
        return false;
    }

    value = _items[_front];

    return true;
}


bool Queue::confirmDequeue()
{
    if (isEmpty())
    {
        return false;
    }

    _front = (_front + 1) % QUEUE_CAPACITY;

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