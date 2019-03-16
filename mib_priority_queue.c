#include "mib_priority_queue.h"

static void mib_priority_queue_swap(struct PriorityQueue* p, size_t a, size_t b)
{
	struct PriorityQueueProp temp = p->buffer[a];
	p->buffer[a] = p->buffer[b];
	p->buffer[b] = temp;
}

static void mib_priority_queue_heapify(struct PriorityQueue* p, size_t n, size_t index)
{
	size_t largest = index;
	size_t left = 2*index + 1;
	size_t right = 2*index + 2;
	
	if(left < n){
		if(p->buffer[largest].priority < p->buffer[left].priority)
			largest = left;
	}

	if(right < n){
		if(p->buffer[largest].priority < p->buffer[right].priority)
			largest = right;
	}

	if(largest != index){
		mib_priority_queue_swap(p,index,largest);
		mib_priority_queue_heapify(p,n,largest);
		
	}
}	

void mib_priority_queue_init(struct PriorityQueue* p)
{
	p->bufferPos = 0;
}

size_t mib_priority_queue_size(struct PriorityQueue* p)
{
	return p->bufferPos;
}

struct PriorityQueueProp* mib_priority_queue_top(struct PriorityQueue* p)
{
	return &p->buffer[0];
}

void mib_priority_queue_pop(struct PriorityQueue* p)
{
	if(mib_priority_queue_size(p) == 0) return;

	mib_priority_queue_swap(p,0,p->bufferPos);
	--p->bufferPos;
	mib_priority_queue_heapify(p,0,p->bufferPos);
}

void mib_priority_queue_push(struct PriorityQueue* p, struct PriorityQueueProp prop)
{
	p->buffer[p->bufferPos] = prop; 
	mib_priority_queue_heapify(p, p->bufferPos + 1, p->bufferPos);
	++p->bufferPos;
}

