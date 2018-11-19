#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "scheduler.h"
#include "mib_queue.h"
#include "mib_queue_codel.h"
#include "rlc_buffer.h"
#include <unistd.h>

#define NUM_QUEUES 1024

static int const queue_num = 0;
static int endThread = 1;
static pthread_t thread_fill_RLC_Buffer;
static pthread_t thread_MAC_sched;
static const int forwardPacket = 1;

static struct QueueCodel* queues[1024];
static struct QueueCodel* priorityQueue;
static struct RLCBuffer* rlcB;

static const int maxNumberPacketsRLC; 

static void (*send_verdict_cb)(uint32_t, uint32_t, uint32_t);

static void init_queues(void(*verdict)(uint32_t, uint32_t, uint32_t))
{
  mib_queue_codel_init(priorityQueue, verdict);
  for(uint32_t i = 0; i < NUM_QUEUES; ++i){
    mib_queue_codel_init(queues[i], verdict); 
  }
}

static void init_rlc_buffer()
{
	rlcB = malloc(sizeof(struct RLCBuffer));
	initRLCBuffer(rlcB);
}

// decide from which queue to take the next packet
static uint32_t* getPacketFromQueues()
{

	return 0;
}

static void* func_fill_RLC_Buffer(void* notUsed)
{
	while(endThread){
		usleep(2000);
		if(getRLCBufferStatus(rlcB) <= maxNumberPacketsRLC ) continue;

		uint32_t* packet = getPacketFromQueues();
		addPacketToRLCBuffer(rlcB,packet);
	}
	return NULL;
}

static void* func_MAC_sched(void *notUsed)
{
	while(endThread){
		usleep(10000);
		sched_yield();

		// loop throug the list of active queues taking into account the 
		uint32_t queueSize = getRLCBufferStatus(rlcB);
		printf("tcp_size = %d\n", queueSize ); 
		uint32_t counter = 0;
		while( (counter < 10) && queueSize != 0){

			uint32_t* idP = getRLCPacket(rlcB);	
			if(idP == NULL)
				break;

			send_verdict_cb(queue_num, *(uint32_t*)idP,forwardPacket);
			free(idP);	
			++counter;
			--queueSize;
		}
	}
	return NULL;
}

void init_sched( void(*verdict)(uint32_t, uint32_t, uint32_t))
{
	send_verdict_cb = verdict;
  init_queues(verdict);
	init_rlc_buffer();
	pthread_create(&thread_fill_RLC_Buffer , NULL, func_MAC_sched, NULL );
	pthread_create(&thread_MAC_sched, NULL, func_fill_RLC_Buffer, NULL );
}

void close_sched()
{
	endThread = 0;
	void* ret = NULL;
	if(pthread_join(thread_MAC_sched, &ret))
		printf("Error joining the thread \n");
	if(pthread_join(thread_fill_RLC_Buffer, &ret))
		printf("Error joining the thread \n");
}

void add_packet_sched(uint32_t queue, uint32_t id, uint32_t hash)
{
	if(queue != queue_num)
		printf("Errro assigning the queue, just queue 0 working!!! \n");

	uint32_t* idP = malloc(sizeof(uint32_t));
	*idP = id;
	uint32_t arrPos = hash % NUM_QUEUES;
	mib_queue_codel_enqueu(queues[arrPos],idP);
	//mib_queue_enqueu(idP);
}

