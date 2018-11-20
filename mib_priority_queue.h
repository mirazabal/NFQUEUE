#ifndef MIB_PRIORITY_QUEUE
#define MIB_PRIORITY_QUEUE

#define PRIORITY_QUEUE_BUCKET 1024

#include <stdint.h>
#include <stddef.h>

struct PriorityQueueProp
{
	uint16_t maxNumPackets;
	uint8_t priority;
	uint8_t queuePos;
};

struct PriorityQueue
{
	struct PriorityQueueProp buffer[PRIORITY_QUEUE_BUCKET];
	size_t bufferPos;
};

void mib_priority_queue_init(struct PriorityQueue* p);

size_t mib_priority_queue_size(struct PriorityQueue* p);

struct PriorityQueueProp* mib_priority_queue_top(struct PriorityQueue* p);

void mib_priority_queue_pop(struct PriorityQueue* p);

void mib_priority_queue_push(struct PriorityQueue* p, struct PriorityQueueProp prop);

#endif

