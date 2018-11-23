#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "scheduler.h"
#include "mib_queue.h"
#include "mib_queue_codel.h"
#include "qfi_buffer.h"
#include "mib_priority_queue.h"
#include "mib_time.h"
#include <unistd.h>

#define NUM_QUEUES 1024

static int init_vars = 0;
static int const queue_num = 0;
static int endThread = 1;
static pthread_t pthread_UPF_sched;
static pthread_t pthread_SDAP_sched;
static const int forwardPacket = 1;

static struct QFIBuffer* qfiB;
static const int maxNumberPacketsQFI = 50; 

static int priorityArr[NUM_QUEUES];  
static int maxNumPackArr[NUM_QUEUES];
//static struct QueueCodel* queues[NUM_QUEUES];
static struct LockFreeQueue* queues[NUM_QUEUES];


static void (*send_verdict_cb)(uint32_t, uint32_t, uint32_t);

static void init_queues(void(*verdict)(uint32_t, uint32_t, uint32_t))
{
  for(uint32_t i = 0; i < NUM_QUEUES; ++i){
   	queues[i] = malloc(sizeof(struct QueueCodel));
	 	//mib_queue_codel_init(queues[i], verdict); 
	 	mib_queue_init(queues[i]); 
		priorityArr[i] = 1;  
		maxNumPackArr[i] = 10;
  }
	priorityArr[0] = 100;  
	init_vars = 1;
}

static void init_qfi_buffer()
{
	qfiB = malloc(sizeof(struct QFIBuffer));
	initQFIBuffer(qfiB);
}

// decide from which queue to take the next packet
static uint32_t* getPacketFromQueues(struct PriorityQueue* pq)
{
	if(mib_priority_queue_size(pq) == 0)
		return NULL;

	struct PriorityQueueProp* prop = mib_priority_queue_top(pq);
//	printf("Priority of the prop == %u \n", prop->priority);
	uint32_t* ret = mib_queue_deque(queues[prop->queuePos]);
	while(ret == NULL){ // the number may not reflect actual status in the queue, since LFDS
		mib_priority_queue_pop(pq);
		if(mib_priority_queue_size(pq) == 0)
			return NULL;

		prop = mib_priority_queue_top(pq);
		ret = mib_queue_deque(queues[prop->queuePos]);
	}

	prop->maxNumPackets--;
	if(prop->maxNumPackets == 0){
		mib_priority_queue_pop(pq);
	}

	//printf("Into getpacket from queues with ret == %lu", *ret);
	return ret;
}

static struct PriorityQueue generatePriorityQueue()
{
	struct PriorityQueue pq;
	mib_priority_queue_init(&pq);
	for(int32_t i = 0; i < NUM_QUEUES; ++i){
		if(mib_queue_size(queues[i]) > 0){

			printf("UPF queue %d with size %lu , at timestamp = %ld \n", i, mib_queue_size(queues[i]), mib_get_time_us() ); 
			struct PriorityQueueProp prop;
			prop.queuePos = i;
			//printf("Position of the queue == %d \n",i);
			prop.priority = priorityArr[i];  
			prop.maxNumPackets = maxNumPackArr[i];
			mib_priority_queue_push(&pq,prop);
		}
	}
	return pq;
}

static void* thread_UPF_sched(void* notUsed)
{
	uint32_t* buff[10];
	int8_t pos = 0;
	while(endThread){
		usleep(1000);
		if(getQFIBufferStatus(qfiB) >= maxNumberPacketsQFI ) continue;
	
		struct PriorityQueue pq = generatePriorityQueue();

//		printf("after generate priority queues %d \n", mib_priority_queue_size(&pq)); //  push(&pq,prop);
		for(pos = -1; pos < 10; ++pos){
			uint32_t* packet = getPacketFromQueues(&pq);
			if(packet == NULL) break;
			buff[pos+1] = packet;
		}
		while(pos > -1){
			addPacketToQFIBuffer(qfiB, buff[pos]);
			--pos;
		}
	}
	return NULL;
}

static void* thread_SDAP_sched(void *notUsed)
{
	while(endThread){
		usleep(10000);
		sched_yield();

	// loop throug the list of active queues taking into account the 
		uint32_t queueSize = getQFIBufferStatus(qfiB);
		printf("QFI queue size = %d at timestamp = %ld \n", queueSize , mib_get_time_us() ); 
		uint32_t counter = 0;
		
		while( (counter < 10) && queueSize != 0){

			uint32_t* idP = getQFIPacket(qfiB);	
			if(idP == NULL)
				break;

			//printf("Packet deque %lu \n",*idP);
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
	init_qfi_buffer();
	pthread_create(&pthread_SDAP_sched , NULL, thread_SDAP_sched, NULL );
	pthread_create(&pthread_UPF_sched, NULL, thread_UPF_sched, NULL );
}

void close_sched()
{
	endThread = 0;
	void* ret = NULL;
	if(pthread_join(pthread_SDAP_sched, &ret))
		printf("Error joining the thread \n");
	if(pthread_join(pthread_UPF_sched, &ret))
		printf("Error joining the thread \n");
}

void add_packet_sched(uint32_t queue, uint32_t id, uint32_t hash)
{
	if(queue != queue_num)
		printf("Errro assigning the queue, just queue 0 working!!! \n");

	if(init_vars == 0)
	 	return;

	uint32_t* idP = malloc(sizeof(uint32_t));
	*idP = id;
	//printf("Packet enqueed %lu \n",*idP);
	uint32_t arrPos = 0;

	if(hash != 0){
		arrPos = hash % 64;// 128;
		if(arrPos ==0)
		 	++arrPos;
	}
	//	printf("queue size before enqueuing = %lu \n", mib_queue_size(queues[arrPos]));
	mib_queue_enqueu(queues[arrPos],idP);
}

