#include "mib_dq.h"
#include "mib_drb_queues.h"
#include "mib_MAC_sched.h"
#include "mib_time.h"

#include <sched.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>
#include <stdio.h>

#define MAC_NUM_PACKETS_PER_TICK 10

static const int forwardPacket = 1;
static int endThread = 1;

struct packetAndQueue
{
	uint32_t* packet;
	uint32_t queueIdx; 
};

static inline void reset_active_queues(int* arrActiveQueues)
{
	for(int i = 0; i < DRB_NUM_QUEUES; ++i)
	{
		 arrActiveQueues[i] = -1;
	}
}

static uint8_t getActiveDRBQueues(struct DRB_queues* drbQ, int* arrActiveQueues)
{
	uint8_t idx = 0;
	for(int queueIdx = 0; queueIdx < DRB_NUM_QUEUES; ++queueIdx )
	{
		uint32_t queueSize = getDRBBufferStatus(drbQ, queueIdx);
		if(queueSize == 0) continue;

	 	arrActiveQueues[idx] = queueIdx;
		++idx;
	}
	return idx;
}

static inline void printDRBStatus(struct DRB_queues* drbQ, uint8_t numActiveQueues, int* arrActiveQueues)
{
	for(int i = 0 ; i <  numActiveQueues; ++i){
		uint32_t queueIdx = arrActiveQueues[i];
		uint32_t queueSize = getDRBBufferStatus(drbQ, queueIdx);
		printf("DRB queue idx = %d with size = %d at timestamp = %ld \n", queueIdx, queueSize, mib_get_time_us()); 
	}
}


static uint8_t selectDRBPacket(struct DRB_queues* drbQ, uint8_t numActiveQueues, struct packetAndQueue* packetsSelected, uint8_t numberPackets, int* arrActiveQueues)
{
	printDRBStatus(drbQ, numActiveQueues, arrActiveQueues);

	uint8_t packetsAlreadySelected = 0;
	while(packetsAlreadySelected < numberPackets && numActiveQueues != 0){
		for(int i = 0; i < numActiveQueues; ++i)
		{
			uint32_t queueIdx = arrActiveQueues[i];
			uint32_t* p = getDRBPacket(drbQ, queueIdx);	
			uint32_t queueSize = getDRBBufferStatus(drbQ, queueIdx);
			if(queueSize == 0){
				arrActiveQueues[i] = arrActiveQueues[numActiveQueues - 1];
				--numActiveQueues;
			}
			
			if(p == NULL) { // can happen... needs more deep inspection
				free(p);
				continue;
			}

			packetsSelected[packetsAlreadySelected].packet = p;
			packetsSelected[packetsAlreadySelected].queueIdx = queueIdx;
			++packetsAlreadySelected; 
		}
	}
	return packetsAlreadySelected; 
}

void close_MAC_thread()
{
  endThread = 0;
}

void* thread_MAC_sched(void* threadData)
{
  struct MAC_thread_data* data = (struct MAC_thread_data*)threadData;   
	struct packetAndQueue dequePackets[MAC_NUM_PACKETS_PER_TICK];
	int arrActiveQueues[DRB_NUM_QUEUES];
	reset_active_queues(arrActiveQueues);

	while(endThread){
		usleep(10000);
    sched_yield();

		uint8_t numPackets = MAC_NUM_PACKETS_PER_TICK;
		uint8_t numActQueues = getActiveDRBQueues(data->drbQ,arrActiveQueues);
		uint8_t numPacSel = selectDRBPacket(data->drbQ, numActQueues, dequePackets, numPackets, arrActiveQueues);

		uint64_t pacDeq = 0;
		for(uint8_t i = 0; i < numPacSel; ++i)
		{
      data->send_verdict_cb(data->NFQUEUE_NUM, *dequePackets[i].packet,forwardPacket);
      free(dequePackets[i].packet);	
			++pacDeq;
		}
		assert(pacDeq < MAC_NUM_PACKETS_PER_TICK + 1);
		mib_dq_dequeued(data->drbQ->dq[0], pacDeq);
	}
  return NULL;
}

