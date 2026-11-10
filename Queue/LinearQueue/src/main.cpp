#include <Arduino.h>
#include "Queue.h"

Queue queue;

/************************************/

void printQueue()
{
    Serial.printf(
        "Size: %u | Full: %s | Empty: %s | Dropped: %u\n",
        static_cast<unsigned>(queue.size()),
        queue.isFull() ? "Yes" : "No",
        queue.isEmpty() ? "Yes" : "No",
        static_cast<unsigned>(queue.droppedCount()));
}

/************************************/

void setup()
{
    Serial.begin(115200);

    Serial.println("\n--- Queue Test ---");

    queue.enqueue(10);
    queue.enqueue(20);
    queue.enqueue(30);
    queue.enqueue(40);

    printQueue();

    Serial.println("\n--- Overflow Test ---");

    queue.enqueue(50);

    printQueue();

    Serial.println("\n--- Dequeue Test ---");

    int value;

    if (queue.peek(value))
    {
        Serial.printf("Processing value: %d\n", value);

        // Simulate successful processing
        bool processed = true;

        if (processed)
        {
            queue.confirmDequeue();
        }
    }
    printQueue();

    Serial.println("\n--- Failed Processing Test ---");

    if (queue.peek(value))
    {
        Serial.printf("Processing value: %d\n", value);

        // Simulate processing failure
        bool processed = false;

        if (processed)
        {
            queue.confirmDequeue();
        }
        else
        {
            Serial.println("Processing failed. Event remains in queue.");
        }
    }

    printQueue();

    Serial.println("\n--- Multi-cycle Queue Test ---");

    Queue stressQueue;

    // Cycle 1: Fill and empty
    Serial.println("\nCycle 1:");

    for (int i = 1; i <= 4; i++)
    {
        stressQueue.enqueue(i);
    }

    while (stressQueue.peek(value))
    {
        Serial.printf("Value: %d\n", value);
        stressQueue.confirmDequeue();
    }

    Serial.printf("Size: %u\n",
                  static_cast<unsigned>(stressQueue.size()));

    // Cycle 2: Fill and empty again
    Serial.println("\nCycle 2:");

    for (int i = 5; i <= 8; i++)
    {
        stressQueue.enqueue(i);
    }

    while (stressQueue.peek(value))
    {
        Serial.printf("Value: %d\n", value);
        stressQueue.confirmDequeue();
    }

    // Cycle 3: Overflow test
    Serial.println("\nCycle 3: Overflow");

    for (int i = 9; i <= 14; i++)
    {
        stressQueue.enqueue(i);
    }

    Serial.printf(
        "Size: %u | Dropped: %u\n",
        static_cast<unsigned>(stressQueue.size()),
        static_cast<unsigned>(stressQueue.droppedCount()));

    // Read remaining elements
    while (stressQueue.peek(value))
    {
        Serial.printf("Value: %d\n", value);
        stressQueue.confirmDequeue();
    }

    // Cycle 4: Reuse after overflow
    Serial.println("\nCycle 4:");

    stressQueue.enqueue(15);
    stressQueue.enqueue(16);

    while (stressQueue.peek(value))
    {
        Serial.printf("Value: %d\n", value);
        stressQueue.confirmDequeue();
    }

    Serial.printf(
        "Final Size: %u | Total Dropped: %u\n",
        static_cast<unsigned>(stressQueue.size()),
        static_cast<unsigned>(stressQueue.droppedCount()));

    Serial.println("\n--- Processing Failure Test ---");

    Queue retryQueue;

    retryQueue.enqueue(100);
    retryQueue.enqueue(200);
    retryQueue.enqueue(300);

    int retryValue;

    // First attempt: processing fails
    if (retryQueue.peek(retryValue))
    {
        Serial.printf("First attempt: %d\n", retryValue);

        bool processed = false;

        if (processed)
        {
            retryQueue.confirmDequeue();
        }
    }

    // Second attempt: check whether the same item remains
    if (retryQueue.peek(retryValue))
    {
        Serial.printf("Second attempt: %d\n", retryValue);

        bool processed = true;

        if (processed)
        {
            retryQueue.confirmDequeue();
        }
    }

    // Check the next item
    if (retryQueue.peek(retryValue))
    {
        Serial.printf("Next item: %d\n", retryValue);
    }

    Serial.printf(
        "Remaining size: %u\n",
        static_cast<unsigned>(retryQueue.size()));
}

/************************************/

void loop()
{
}