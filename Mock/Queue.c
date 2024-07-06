#include <stdint.h>
#include "Queue.h"


uint8_t isFull(Queue_t* queue)
{
    return (queue->size == queue->capacity);
}

uint8_t isEmpty(Queue_t* queue)
{
    return (queue->size == 0);
}

void enqueue(Queue_t* queue, uint8_t item)
{
    if (isFull(queue))
        return;
    queue->rear = (queue->rear + 1)
                  % queue->capacity;
    queue->QueueData[queue->rear] = item;
    queue->size = queue->size + 1;
}

uint8_t dequeue(Queue_t* queue)
{
    if (isEmpty(queue))
        return 0;
    uint8_t item = queue->QueueData[queue->front];
    queue->front = (queue->front + 1)
                   % queue->capacity;
    queue->size = queue->size - 1;
    return item;
}


uint8_t front(Queue_t* queue)
{
    if (isEmpty(queue))
        return 0;
    return queue->QueueData[queue->front];
}


uint8_t rear(Queue_t* queue)
{
    if (isEmpty(queue))
        return 0;
    return queue->QueueData[queue->rear];
}
