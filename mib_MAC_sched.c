#include "mib_MAC_sched.h"
#include "mib_drb_queues.h"
#include "mib_time.h"

#include <sched.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>
#include <stdio.h>

static const int forwardPacket = 1;
static int endThread = 1;

static int arrActiveQueues[DRB_NUM_QUEUES];

struct packetAndQueue
{
	uint32_t* packet;
	uint32_t queueIdx; 
};

static struct packetAndQueue packetsSelected[10];

static void getActiveDRBQueues(struct DRB_queues* drbQ)
{
	for(int i = 0; i < DRB_NUM_QUEUES; ++i)
	{
	 arrActiveQueues[i] = -1;
	}
	uint8_t idx = 0;
	for(int queueIdx = 0; queueIdx < DRB_NUM_QUEUES; ++queueIdx )
	{
		uint32_t queueSize = getDRBBufferStatus(drbQ, queueIdx);
		if(queueSize == 0) continue;

	 	arrActiveQueues[idx] = queueIdx;
		++idx;
	}
}

static void selectDRBPacket(struct DRB_queues* drbQ)
{
	for(int i =0 ; i < 10; ++i){
	 packetsSelected[i].packet = NULL;
	}

 	int packetsAlreadySelected = 0;
	int noPacketDetected = 1;
	while(packetsAlreadySelected < 10 && noPacketDetected == 1){
		noPacketDetected = 0;
		for(int i = 0; i < DRB_NUM_QUEUES; ++i)
		{
			if(arrActiveQueues[i] == -1) continue;
			uint32_t queueIdx = arrActiveQueues[i];
			uint32_t queueSize = getDRBBufferStatus(drbQ, queueIdx);
			if(queueSize == 0){
				arrActiveQueues[i] = -1;
				continue;
			}
			noPacketDetected = 1;
 			uint32_t* p = getDRBPacket(drbQ, queueIdx);	
//			printf("QFI queue = %d size = %d at timestamp = %ld \n", queueIdx, queueSize , mib_get_time_us() ); 

    	printf("DRB queue idx = %d with size = %d at timestamp = %ld \n", queueIdx,queueSize , mib_get_time_us() ); 
			packetsSelected[packetsAlreadySelected].packet = p;
			packetsSelected[packetsAlreadySelected].queueIdx = queueIdx;
			++packetsAlreadySelected; 
		}
	}
}


void close_MAC_thread()
{
  endThread = 0;
}

void* thread_MAC_sched(void* threadData)
{
  struct MAC_thread_data* data = (struct MAC_thread_data*)threadData;   

	while(endThread){
		usleep(10000);
    sched_yield();

	//	uint32_t queueSize = getDRBBufferStatus(data->drbQ, QUEUE_DRB);
  //  printf("DRB queue size = %d at timestamp = %ld \n", queueSize , mib_get_time_us() ); 
		
	//	if(getDRBBufferStatus(data->drbQ, DRB_QUEUE_IDX) >= maxNumberPacketsDRB)
	//			continue;

		getActiveDRBQueues(data->drbQ);
		selectDRBPacket(data->drbQ);
		for(int i = 0; i < 10; ++i)
		{
			if(packetsSelected[i].packet == NULL) break;

      data->send_verdict_cb(data->NFQUEUE_NUM, *packetsSelected[i].packet,forwardPacket);
      free(packetsSelected[i].packet);	
		}
	}

/*
//  uint8_t QUEUE_DRB = 0;
  while(endThread){
    usleep(10000);
    sched_yield();

    // loop throug the list of active queues taking into account the 
    uint32_t queueSize = getDRBBufferStatus(data->drbQ, QUEUE_DRB);
    printf("DRB queue size = %d at timestamp = %ld \n", queueSize , mib_get_time_us() ); 
    uint32_t counter = 0;
    while((counter < 10) && queueSize != 0){

      uint32_t* idP = getDRBPacket(data->drbQ,QUEUE_DRB);	
      if(idP == NULL)
        break;

      data->send_verdict_cb(data->NFQUEUE_NUM, *(uint32_t*)idP,forwardPacket);
      free(idP);	
      ++counter;
      --queueSize;
    }
  }
	*/
  return NULL;

}

