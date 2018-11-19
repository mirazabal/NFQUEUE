

#include "scheduler.h"
#include "mib_queue.h"

#define NUM_QUEUES 1024

static int const queue_num = 0;
static int endThread = 1;
static pthread_t thread_fill_RLC_Buffer;
static pthread_t thread_MAC_sched;
static const int forwardPacket = 1;

static struct QueueCodel queues[NUM_QUEUES];
static struct QueueCodel priorityQueue;
static struct RLCBufer* rlcB;

static const int maxNumberPacketsRLC; 

static void (*send_verdict_cb)(uint32_t, uint32_t, uint32_t);

static void init_queues(void(*verdict)(uint32_t, uint32_t, uint32_t))
{
  mib_queue_codel_init(priorityQueue);
  for(uint32_t i = 0; i < NUM_QUEUES; ++i){
    mib_queue_codel_init(queues[i], verdict); 
  }
}


// decide from which queue to take the next packet
static uint32_t getPacketFromQueues()
{


}

static void* thread_fill_RLC_Buffer(void* notUsed)
{
	while(endThread){
		usleep(10000);
		sched_yield();


static void* thread_MAC_sched(void *notUsed)
{
  while(endThread){
    usleep(10000);
    sched_yield();


  // loop throug the list of active queues taking into account the 

    uint32_t queueSize = getRLCBufferStatus();
    printf("tcp_size = %d\n", queueSize ); 
    uint32_t counter = 0;
    while( (counter < 10) && queueSize != 0){

      uint32_t* idP = getRLCPacket(rlcB);	
      if(idP == NULL)
        break;

}

void init_sched( void(*verdict)(uint32_t, uint32_t, uint32_t))
{
	send_verdict_cb = verdict;
        init_queues(verdict);
	pthread_create(&thread_sched, NULL, thread_func, NULL );
}

void close_sched()
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
        uint32_t arrPos = hash % NUM_QUEUES;
        mib_queue_codel_enqueu(queues[arrPos],idP);
	//mib_queue_enqueu(idP);
}

