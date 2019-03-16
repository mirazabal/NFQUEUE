#include "mib_dq.h"
#include "mib_drb_queues.h"
#include "mib_SDAP_sched.h"
#include "mib_qfi_queues.h"
#include "mib_time.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>

#define SDAP_NUM_PACKETS_PER_TICK 10

static const int forwardPacket = 1;
static int endThread = 1;
static const uint64_t maxNumberPacketsDRB =1024; 

struct packetAndQueue
{
	uint32_t* packet;
	uint32_t queueIdx; 
};

static uint64_t getMaxNumberPacketsDRB()
{
	return maxNumberPacketsDRB;
}

static inline void reset_active_queues(int* arrActiveQueues)
{
	for(int i = 0; i < QFI_NUM_QUEUES; ++i)
	{
		 arrActiveQueues[i] = -1;
	}
}

static uint8_t getActiveQFIQueues(struct QFI_queues* qfiQ, int* arrActiveQueues)
{
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

static inline void printQFIStatus(struct QFI_queues* qfiQ, uint8_t numActiveQueues, int* arrActiveQueues)
{
	for(int i = 0 ; i <  numActiveQueues; ++i){
		uint32_t queueIdx = arrActiveQueues[i];
		uint32_t queueSize = getQFIBufferStatus(qfiQ, queueIdx);
		printf("QFI queue idx = %d with size = %d at timestamp = %ld \n", queueIdx, queueSize, mib_get_time_us()); 
	}
}

static uint8_t selectQFIPacket(struct QFI_queues* qfiQ, uint8_t numActiveQueues, struct packetAndQueue* packetsSelected, uint8_t numberPackets, int* arrActiveQueues)
{
	printQFIStatus(qfiQ, numActiveQueues, arrActiveQueues);

	uint8_t packetsAlreadySelected = 0;
	while(packetsAlreadySelected < numberPackets && numActiveQueues != 0){
		for(int i = 0; i < numActiveQueues; ++i)
		{
			uint32_t queueIdx = arrActiveQueues[i];
			uint32_t* p = getQFIPacket(qfiQ, queueIdx);	
			uint32_t queueSize = getQFIBufferStatus(qfiQ, queueIdx);
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

void close_SDAP_thread()
{
  endThread = 0;
}
  
void* thread_SDAP_sched(void *threadData)
{
	struct SDAP_thread_data* data = (struct SDAP_thread_data*)threadData;
	struct packetAndQueue dequePackets[SDAP_NUM_PACKETS_PER_TICK];
	int arrActiveQueues[QFI_NUM_QUEUES];
	reset_active_queues(arrActiveQueues);

	const uint8_t DRB_QUEUE_IDX = 0;
	while(endThread){
		usleep(1000);
		//uint32_t packetsAtDRB = getDRBBufferStatus(data->drbQ, DRB_QUEUE_IDX); 
		//if(packetsAtDRB >= getMaxNumberPacketsDRB())
		//		continue;
		//uint32_t numPackets =  getMaxNumberPacketsDRB() - packetsAtDRB < SDAP_NUM_PACKETS_PER_TICK + 1 ? getMaxNumberPacketsDRB() - packetsAtDRB : SDAP_NUM_PACKETS_PER_TICK;
		
		uint64_t remainingPac = get_DRB_avail(data->drbQ, DRB_QUEUE_IDX); 
		if(remainingPac == 0) continue;

		uint8_t numPackets = remainingPac < SDAP_NUM_PACKETS_PER_TICK + 1 ? remainingPac : SDAP_NUM_PACKETS_PER_TICK;	
		uint8_t numActQueues = getActiveQFIQueues(data->qfiQ, arrActiveQueues);
		uint8_t numPacSel = selectQFIPacket(data->qfiQ, numActQueues, dequePackets, numPackets, arrActiveQueues);
		uint8_t pacEnq = 0;
		for(uint8_t i = 0 ; i < numPacSel; ++i,++pacEnq)
		{
				addPacketToDRB(data->drbQ, DRB_QUEUE_IDX, dequePackets[i].packet);
		}
  	mib_dq_enqueued(data->drbQ->dq[DRB_QUEUE_IDX], pacEnq);
	}
	return NULL;
}

