#define MAX_NUM_PACKETS 2048

#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include "scheduler.h"

static int const queue_num = 0;
static int tcp_id[MAX_NUM_PACKETS]; 
static int tcp_it_begin = 0;
static int tcp_it_end = 0;

static pthread_mutex_t lock;
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
		pthread_mutex_lock(&lock);

		printf("tcp_size = %d\n", tcp_it_end-tcp_it_begin ); 
		if(tcp_it_end == tcp_it_begin )
		{
		 	goto end_loop;
		}

		uint32_t counter = 0;
		while( (counter < 10) && tcp_it_begin != tcp_it_end){
			if( tcp_it_begin < 0 || tcp_it_begin >  MAX_NUM_PACKETS -1)
				printf("tcp_begin value out of range == %d, \n", tcp_it_begin);
				
			//printf("value before send veredict == %d, with it_begin  %d  nad it_end %d \n", tcp_id[tcp_it_begin], tcp_it_begin, tcp_it_end);
				
			send_verdict_cb( queue_num,tcp_id[tcp_it_begin],1);
			tcp_id[tcp_it_begin] = -1;
			counter++;
			tcp_it_begin = get_it_val(tcp_it_begin);
		}
end_loop:
		pthread_mutex_unlock(&lock); 
		//pthread_yield();
	}	
	return NULL;
}


void init_sched( void(*verdict)(uint32_t, uint32_t, uint32_t))
{
	send_verdict_cb = verdict;
	tcp_it_begin = 0;
	tcp_it_end = 0;

	for(int i = 0; i < MAX_NUM_PACKETS; i++){
		tcp_id[i] = -1;
	}

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

	pthread_mutex_lock(&lock);
/*	if(hash == 0){
		tcp_priority[tcp_priority_pos] = queue; 
		tcp_priority_2[tcp_priority_pos] = id; 
		tcp_priority_pos++;
	} else {*/
		
	if( tcp_it_end < 0 || tcp_it_end >  MAX_NUM_PACKETS -1)
			printf("tcp_end value out of range == %d, \n", tcp_it_end);
		
		tcp_id[tcp_it_end] = id;
		tcp_it_end = get_it_val(tcp_it_end);

//	}
	pthread_mutex_unlock(&lock);
}

