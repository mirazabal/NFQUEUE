#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include "scheduler.h"
//#include "mib_queue.h"
#include "mib_queue_codel.h"

#define MAX_NUM_PACKETS 2048
#define NUM_QUEUES 1024

static int const queue_num = 0;
static int endThread = 1;
static pthread_t thread_sched;
static const int forwardPacket = 1;

static void (*send_verdict_cb)(uint32_t, uint32_t, uint32_t);


struct mib_code_queue
{

};

struct mib_scheduler_queues
{

	static mib_code_queue queues_[NUM_QUEUES]; 
	
}




static void* sched_thread(void *notUsed)
{
	while(endThread){
		usleep(10000);
		sched_yield();

		uint32_t queueSize = mib_queue_codel_size();
		printf("queue size = %d\n", queueSize ); 

		uint32_t counter = 0;
		while( (counter < 10) && queueSize != 0){
			uint32_t* idP = mib_queue_codel_deque();	
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
	mib_queue_codel_init(verdict);
	pthread_create(&thread_sched, NULL, sched_thread, NULL );
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
	if(queue != queue_num)
		printf("Errro assigning the queue, just queue 0 working!!! \n");
	
	
	uint32_t* idP = malloc(sizeof(uint32_t));
	*idP = id;
	mib_queue_codel_enqueu(idP);
}

