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

struct packetAndQueue
{
	uint32_t* packet;
	uint32_t queueIdx; 
};

static void getActiveDRBQueues(struct DRB_queues* drbQ, int* arrActiveQueues)
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

static void selectDRBPacket(struct DRB_queues* drbQ,  struct packetAndQueue* packetsSelected, uint8_t numPacketsPerTick, int* arrActiveQueues)
{
	for(int i =0 ; i < numPacketsPerTick; ++i){
	 packetsSelected[i].packet = NULL;
	}

 	int packetsAlreadySelected = 0;
	int packetDetected = 1;
	while(packetsAlreadySelected < numPacketsPerTick && packetDetected == 1){
			packetDetected = 0;
		for(int i = 0; i < DRB_NUM_QUEUES; ++i)
		{
			if(arrActiveQueues[i] == -1) continue;
			uint32_t queueIdx = arrActiveQueues[i];
			uint32_t queueSize = getDRBBufferStatus(drbQ, queueIdx);
			if(queueSize == 0){
				arrActiveQueues[i] = -1;
				continue;
			}
			packetDetected = 1;
 			uint32_t* p = getDRBPacket(drbQ, queueIdx);	
//			printf("QFI queue = %d size = %d at timestamp = %ld \n", queueIdx, queueSize , mib_get_time_us() ); 

    	printf("DRB queue idx = %d with size = %d at timestamp = %ld \n", queueIdx,queueSize , mib_get_time_us() ); 
			packetsSelected[packetsAlreadySelected].packet = p;
			packetsSelected[packetsAlreadySelected].queueIdx = queueIdx;
			++packetsAlreadySelected; 
		}
	}
}

struct packetAndQueue* init_num_packets_process(uint8_t numPackets)
{
  struct packetAndQueue* packetsSelected = malloc(sizeof(struct packetAndQueue)*numPackets);
	return packetsSelected;
}

void close_num_packets_process( struct packetAndQueue* packetsSelected)
{
	free(packetsSelected);
}

void close_MAC_thread()
{
  endThread = 0;
}

void* thread_MAC_sched(void* threadData)
{
  struct MAC_thread_data* data = (struct MAC_thread_data*)threadData;   
	const uint8_t numPacketsPerTick = 10;
  struct packetAndQueue* packetsPerTick = init_num_packets_process(numPacketsPerTick);

	int arrActiveQueues[DRB_NUM_QUEUES];

	while(endThread){
		usleep(10000);
    sched_yield();

		getActiveDRBQueues(data->drbQ,arrActiveQueues);
		selectDRBPacket(data->drbQ,packetsPerTick, numPacketsPerTick,arrActiveQueues);
		for(int i = 0; i < numPacketsPerTick; ++i)
		{
			if(packetsPerTick[i].packet == NULL) break;

      data->send_verdict_cb(data->NFQUEUE_NUM, *packetsPerTick[i].packet,forwardPacket);
      free(packetsPerTick[i].packet);	
		}
	}
 close_num_packets_process(packetsPerTick);
  return NULL;
}

