#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "mib_UPF_sched.h"
#include "mib_priority_queue.h"
#include "mib_time.h"
#include "mib_queue_codel.h"
#include "mib_qfi_queues.h"
#include "mib_upf_queues.h"

#define NUM_QUEUES 1024

static const int maxNumberPacketsQFI = 50; 
static int priorityArr[NUM_QUEUES];  
static int maxNumPackArr[NUM_QUEUES];
static int endThread = 1;

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

// decide from which queue to take the next packet
static uint32_t* getPacketFromUPF(struct UPF_queues* upfQ, struct PriorityQueue* pq)
{
  if(mib_priority_queue_size(pq) == 0)
    return NULL;

  struct PriorityQueueProp* prop = mib_priority_queue_top(pq);
  //	printf("Priority of the prop == %u \n", prop->priority);
  uint32_t* ret = mib_queue_deque(upfQ->queues[prop->queuePos]);
  while(ret == NULL){ // the number may not reflect actual status in the queue, since LFDS
    mib_priority_queue_pop(pq);
    if(mib_priority_queue_size(pq) == 0)
      return NULL;

    prop = mib_priority_queue_top(pq);
    ret = mib_queue_deque(upfQ->queues[prop->queuePos]);
  }

  prop->maxNumPackets--;
  if(prop->maxNumPackets == 0){
    mib_priority_queue_pop(pq);
  }

  //printf("Into getpacket from queues with ret == %lu", *ret);
  return ret;
}

void close_UPF_thread()
{
  endThread = 0;
}

void* thread_UPF_sched(void* threadData)
{
  struct UPF_thread_data* data = (struct UPF_thread_data*)threadData;

  init_priority_max_quantity(data);

  uint32_t* buff[10];
  int8_t pos = 0;
  const uint8_t QUEUE_QFI = 0;
  assert(data->qfiQ->queues[QUEUE_QFI] != NULL);
  assert(data->upfQ->queues[QUEUE_QFI] != NULL);

  while(endThread){
    usleep(1000);
    if(getQFIBufferStatus(data->qfiQ, QUEUE_QFI) >= maxNumberPacketsQFI )
      continue;

    struct PriorityQueue pq = generatePriorityQueue(data->upfQ);

    //printf("after generate priority queues %d \n", mib_priority_queue_size(&pq)); //  push(&pq,prop);
    for(pos = -1; pos < 10; ++pos){
      uint32_t* packet = getPacketFromUPF(data->upfQ, &pq);
      if(packet == NULL) break;
      buff[pos+1] = packet;
    }
    while(pos > -1){
      addPacketToQFI(data->qfiQ, QUEUE_QFI, buff[pos]); // this is stupid!!!
      --pos;
    }
  }
  return NULL;
}

