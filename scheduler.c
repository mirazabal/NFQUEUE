#define MAX_NUM_PACKETS 2048

#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>


#include <unistd.h>
#include "scheduler.h"
#include "mib_queue.h"

static int const queue_num = 0;
//static int tcp_id[MAX_NUM_PACKETS]; 
//static int tcp_it_begin = 0;
//static int tcp_it_end = 0;
//static pthread_mutex_t lock;

static int endThread = 1;
static pthread_t thread_sched;
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
//		pthread_mutex_lock(&lock);

		uint32_t queueSize = mib_queue_size();
		printf("tcp_size = %d\n", queueSize ); 
		uint32_t counter = 0;
		while( (counter < 10) && queueSize != 0){
				
			uint32_t* idP = mib_queue_deque();	
			if(idP == NULL)
				break;
			//printf("value of idP = %d", *idP);
			send_verdict_cb( queue_num,*idP,1);
			free(idP);
			++counter;
			--queueSize;
		}
//		pthread_mutex_unlock(&lock); 
	}	
	return NULL;
}


void init_sched( void(*verdict)(uint32_t, uint32_t, uint32_t))
{
	send_verdict_cb = verdict;
	
	/*tcp_it_begin = 0;
	tcp_it_end = 0;

	for(int i = 0; i < MAX_NUM_PACKETS; i++){
		tcp_id[i] = -1;
	}
	*/
	mib_queue_init();
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
	if(queue != queue_num)
		printf("Errro assigning the queue, just queue 0 working!!! \n");

//	pthread_mutex_lock(&lock);
/*	if(hash == 0){
		tcp_priority[tcp_priority_pos] = queue; 
		tcp_priority_2[tcp_priority_pos] = id; 
		tcp_priority_pos++;
	} else {*/
		
//	if( tcp_it_end < 0 || tcp_it_end >  MAX_NUM_PACKETS -1)
	//		printf("tcp_end value out of range == %d, \n", tcp_it_end);
		
//		tcp_id[tcp_it_end] = id;
//		tcp_it_end = get_it_val(tcp_it_end);

		uint32_t* idP = malloc(sizeof(uint32_t));
		*idP = id;
		mib_queue_enqueu(idP);
//	}
//	pthread_mutex_unlock(&lock);
}

