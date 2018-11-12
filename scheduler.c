#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include "scheduler.h"
#include "mib_queue.h"

#define MAX_NUM_PACKETS 2048

static int const queue_num = 0;
static int endThread = 1;
static pthread_t thread_sched;
static const int forwardPacket = 1;

static void (*send_verdict_cb)(uint32_t, uint32_t, uint32_t);

static int get_it_val(int it)
{
	it++;
	if(it == MAX_NUM_PACKETS)
		it = 0;
	return it;
}

static void* thread_func(void *notUsed)
{
	while(endThread){
		usleep(10000);
		sched_yield();

		uint32_t queueSize = mib_queue_size();
		printf("tcp_size = %d\n", queueSize ); 
		uint32_t counter = 0;
		while( (counter < 10) && queueSize != 0){

			uint32_t* idP = mib_queue_deque();	
			if(idP == NULL)
				break;

			send_verdict_cb( queue_num, *(uint32_t*)idP,forwardPacket);
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
	mib_queue_init(verdict);
	pthread_create(&thread_sched, NULL, thread_func, NULL );
}

void close_sched()
{
	endThread = 0;
	void* ret = NULL;
	if(pthread_join(thread_sched, &ret))
		printf("Error joining the thread \n");
}

void add_packet_sched(uint32_t queue, uint32_t id, uint32_t hash)
{
	if(queue != 0)
		printf("Errro assigning the queue, just queue 0 working!!! \n");
	uint32_t* idP = malloc(sizeof(uint32_t));
	*idP = id;
	mib_queue_enqueu(idP);
}

