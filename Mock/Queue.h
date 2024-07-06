#ifndef __QUEUE__
#define __QUEUE__


#include <stdint.h>

#define QUEUE_CAPACITY 		99

typedef struct Queue {
    int front, rear, size;
    unsigned capacity;
    uint8_t* QueueData;
} Queue_t;

uint8_t CirQueue_Full ();
uint8_t CirQueue_Empty ();
void 	CirQueue_Push (uint8_t input);
uint8_t CirQueue_Pop ();
uint8_t front(Queue_t* queue);
uint8_t rear(Queue_t* queue);

#endif
