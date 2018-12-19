#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "mib_UPF_sched.h"
#include "mib_priority_queue.h"
#include "mib_time.h"
#include "mib_queue_codel.h"
#include "mib_qfi_queues.h"
#include "mib_upf_queues.h"
#include "mapper.h"

static const int maxNumberPacketsQFI = 1024; 
static int priorityArr[UPF_NUM_QUEUES];  
static int maxNumPackArr[UPF_NUM_QUEUES];
static int endThread = 1;


static void init_mapper(struct mib_mapper* map)
{
	mib_init_mapper(map, UPF_NUM_QUEUES, QFI_NUM_QUEUES);	
	for(int i = 0; i < UPF_NUM_QUEUES; ++i){
		if(i < QFI_NUM_QUEUES){
			mib_set_output_for_input(map, i, i);
		}else{
			mib_set_output_for_input(map, i,  QFI_NUM_QUEUES - 1);
		}
	}	
}

static void init_priority_max_quantity(struct UPF_queues* upfQ)
{
  for(uint32_t i = 0; i < upfQ->sizeQueue; ++i){
    priorityArr[i] = 1;  
    maxNumPackArr[i] = 10;
  }
  priorityArr[0] = 100;  
}

static struct PriorityQueue generatePriorityQueue(struct UPF_queues* upfQ)
{
  struct PriorityQueue pq;
  mib_priority_queue_init(&pq);

  for(int32_t i = 0; i < upfQ->sizeQueue; ++i){

    if(mib_queue_size(upfQ->queues[i]) > 0){
      printf("UPF queue %d with size %lu , at timestamp = %ld \n", i, mib_queue_size(upfQ->queues[i]), mib_get_time_us() ); 
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


struct PacketAndQueuPos
{
	uint32_t* packet;
	uint8_t queuePos;
};

// decide from which queue to take the next packet
static struct PacketAndQueuPos getPacketFromUPF(struct UPF_queues* upfQ, struct PriorityQueue* pq)
{
	
	struct PacketAndQueuPos p;
	p.packet = NULL;
	p.queuePos = 0;

  if(mib_priority_queue_size(pq) == 0)
    return p;

  struct PriorityQueueProp* prop = mib_priority_queue_top(pq);
  //	printf("Priority of the prop == %u \n", prop->priority);
  uint32_t* ret = mib_queue_deque(upfQ->queues[prop->queuePos]);
  while(ret == NULL){ // the number may not reflect actual status in the queue, since LFDS
    mib_priority_queue_pop(pq);
    if(mib_priority_queue_size(pq) == 0)
      return p;

    prop = mib_priority_queue_top(pq);
    ret = mib_queue_deque(upfQ->queues[prop->queuePos]);
  }

  prop->maxNumPackets--;
  if(prop->maxNumPackets == 0){
    mib_priority_queue_pop(pq);
  }

  //printf("Into getpacket from queues with ret == %lu", *ret);
	p.packet = ret;
	p.queuePos = prop->queuePos;
  return p;
}

void close_UPF_thread()
{
  endThread = 0;
}

void* thread_UPF_sched(void* threadData)
{
	struct UPF_thread_data* data = (struct UPF_thread_data*)threadData;

	init_priority_max_quantity(data->upfQ);
	struct mib_mapper map;
	init_mapper(&map);

	while(endThread){
		usleep(1000);
		//   if(getQFIBufferStatus(data->qfiQ, QUEUE_QFI) >= maxNumberPacketsQFI )
		//      continue;

		struct PriorityQueue pq = generatePriorityQueue(data->upfQ);

		for(int8_t pos = 0; pos < 10; ++pos){
			struct PacketAndQueuPos p = getPacketFromUPF(data->upfQ, &pq);
			if(p.packet == NULL) break;

			addPacketToQFI(data->qfiQ, p.queuePos, p.packet); 
		}
	}
	free_mapper(&map);
	return NULL;
}

