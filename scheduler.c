#define NUM_PACKETS 8192

#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include "scheduler.h"

static int tcp_1[NUM_PACKETS]; 
static int tcp_2[NUM_PACKETS]; 
static int tcp_pos = 0;

static int tcp_priority[NUM_PACKETS]; 
static int tcp_priority_2[NUM_PACKETS]; 
static int tcp_priority_pos = 0;

static pthread_mutex_t lock;
static int endThread = 1;
static pthread_t thread_sched;
static void (*send_verdict_cb)(uint32_t, uint32_t, uint32_t);

static void* thread_func(void *notUsed)
{
	while(endThread){
		pthread_mutex_lock(&lock);
	
/*		uint32_t counter_prio = 0;
		while(counter_prio < 100 && tcp_priority[counter_prio] != -1){
			send_verdict_cb(tcp_priority[counter_prio],tcp_priority_2[counter_prio],1);
			tcp_priority[counter_prio] = -1;
			counter_prio++;
		}
		tcp_priority_pos = tcp_priority_pos - counter_prio;
*/
		uint32_t counter = 0; //counter_prio;
		while(counter < 100 && tcp_1[counter] != -1){
			send_verdict_cb(tcp_1[counter],tcp_2[counter],1);
			tcp_1[counter] = -1;
			counter++;
		}
		tcp_pos = tcp_pos - counter;
		pthread_mutex_unlock(&lock); 
		counter = 0;
		usleep(10000);
	}	
}


void init_sched( void(*verdict)(uint32_t, uint32_t, uint32_t))
{
	send_verdict_cb = verdict;
	for(int i = 0; i < NUM_PACKETS; i++){
		tcp_1[i] = -1;
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
	pthread_mutex_lock(&lock);
	if(hash == 0){
		tcp_priority[tcp_priority_pos] = queue; 
		tcp_priority_2[tcp_priority_pos] = id; 
		tcp_priority_pos++;
	} else {
		tcp_1[tcp_pos] = queue;
		tcp_2[tcp_pos] = id;
		tcp_pos++;
	}
	pthread_mutex_unlock(&lock);
}

