#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>

#include "mib_SDAP_sched.h"
#include "mib_qfi_queues.h"
#include "mib_drb_queues.h"
#include "mib_time.h"

#define NUM_PACKETS_PER_TICK 10

static const int forwardPacket = 1;
static int endThread = 1;
static const int maxNumberPacketsDRB =30; 
static int arrActiveQueues[QFI_NUM_QUEUES];


struct packetAndQueue
{
	uint32_t* packet;
	uint32_t queueIdx; 
};


static uint8_t getActiveQFIQueues(struct QFI_queues* qfiQ)
{
	for(int i = 0; i < QFI_NUM_QUEUES; ++i)
	{
	 arrActiveQueues[i] = -1;
	}
	uint8_t idx = 0;
	for(int queueIdx = 0; queueIdx < QFI_NUM_QUEUES; ++queueIdx )
	{
		uint32_t queueSize = getQFIBufferStatus(qfiQ, queueIdx);
		if(queueSize == 0) continue;

	 	arrActiveQueues[idx] = queueIdx;
		++idx;
	}
	return idx;
}

static uint8_t selectQFIPacket(struct QFI_queues* qfiQ, uint8_t activeQueues, struct packetAndQueue* packetsSelected, uint8_t numberPackets)
{
 	uint8_t packetsAlreadySelected = 0;
	while(packetsAlreadySelected < numberPackets && activeQueues != 0){
		for(int i = 0; i < activeQueues; ++i)
		{
			uint32_t queueIdx = arrActiveQueues[i];
			uint32_t* p = getQFIPacket(qfiQ, queueIdx);	
			packetsSelected[packetsAlreadySelected].packet = p;
			packetsSelected[packetsAlreadySelected].queueIdx = queueIdx;
			++packetsAlreadySelected; 

			uint32_t queueSize = getQFIBufferStatus(qfiQ, queueIdx);
			printf("QFI queue idx = %d with size = %d at timestamp = %ld \n", queueIdx, queueSize, mib_get_time_us()); 
			if(queueSize == 0){
				arrActiveQueues[i] = arrActiveQueues[activeQueues - 1];
				--activeQueues;
			}
		}
	}
	return packetsAlreadySelected; 
}

void close_SDAP_thread()
{
  endThread = 0;
}
  
void* thread_SDAP_sched(void *threadData)
{
	struct SDAP_thread_data* data = (struct SDAP_thread_data*)threadData;

	struct packetAndQueue dequePackets[NUM_PACKETS_PER_TICK];
	const uint8_t DRB_QUEUE_IDX = 0;
	while(endThread){
		usleep(1000);
		uint32_t packetsAtDRB = getDRBBufferStatus(data->drbQ, DRB_QUEUE_IDX); 
		if(packetsAtDRB >= maxNumberPacketsDRB)
				continue;

		uint8_t actQueues = getActiveQFIQueues(data->qfiQ);
		uint32_t numPackets =  maxNumberPacketsDRB - packetsAtDRB < NUM_PACKETS_PER_TICK + 1 ? maxNumberPacketsDRB - packetsAtDRB : NUM_PACKETS_PER_TICK;
		uint8_t numPacSel = selectQFIPacket(data->qfiQ, actQueues, dequePackets, numPackets);
		for(int i = 0; i < numPacSel; ++i)
		{
				addPacketToDRB(data->drbQ, DRB_QUEUE_IDX, dequePackets[i].packet);
		}
	}
	return NULL;
}

