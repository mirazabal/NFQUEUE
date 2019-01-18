#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>

#include "mib_SDAP_sched.h"
#include "mib_qfi_queues.h"
#include "mib_drb_queues.h"
#include "mib_time.h"

static const int forwardPacket = 1;
static int endThread = 1;
static const int maxNumberPacketsDRB = 1024; 
static int arrActiveQueues[QFI_NUM_QUEUES];

struct packetAndQueue
{
	uint32_t* packet;
	uint32_t queueIdx; 
};

static struct packetAndQueue packetsSelected[10];

static void getActiveQFIQueues(struct QFI_queues* qfiQ)
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
}

static void selectQFIPacket(struct QFI_queues* qfiQ)
{
	for(int i =0 ; i < 10; ++i){
	 packetsSelected[i].packet = NULL;
	}
 	int packetsAlreadySelected = 0;
	int noPacketDetected = 1;
	while(packetsAlreadySelected < 10 && noPacketDetected == 1){
		noPacketDetected = 0;
		for(int i = 0; i < QFI_NUM_QUEUES; ++i)
		{
			if(arrActiveQueues[i] == -1) continue;
			uint32_t queueIdx = arrActiveQueues[i];
			uint32_t queueSize = getQFIBufferStatus(qfiQ, queueIdx);
			if(queueSize == 0){
				arrActiveQueues[i] = -1;
				continue;
			}
			noPacketDetected = 1;
 			uint32_t* p = getQFIPacket(qfiQ, queueIdx);	
    	printf("QFI queue idx = %d with size = %d at timestamp = %ld \n", queueIdx, queueSize, mib_get_time_us()); 
    	if(queueIdx != 0)
				printf("QFI queue idx = 0 with size = %ld at timestamp = %ld \n",  getQFIBufferStatus(qfiQ, 0), mib_get_time_us()); 
			packetsSelected[packetsAlreadySelected].packet = p;
			packetsSelected[packetsAlreadySelected].queueIdx = queueIdx;
			++packetsAlreadySelected; 
		}
	}
}

void close_SDAP_thread()
{
  endThread = 0;
}
  
void* thread_SDAP_sched(void *threadData)
{
	struct SDAP_thread_data* data = (struct SDAP_thread_data*)threadData;

//	const uint8_t QUEUE_QFI = 0;
	const uint8_t DRB_QUEUE_IDX = 0;
	while(endThread){
		usleep(1000);

		if(getDRBBufferStatus(data->drbQ, DRB_QUEUE_IDX) >= maxNumberPacketsDRB)
				continue;

		getActiveQFIQueues(data->qfiQ);
		selectQFIPacket(data->qfiQ);
		for(int i = 0; i < 10; ++i)
		{
			if(packetsSelected[i].packet == NULL) break;
			//	addPacketToDRB(data->drbQ, (packetsSelected[i].queueIdx) % DRB_NUM_QUEUES, packetsSelected[i].packet);
				addPacketToDRB(data->drbQ, DRB_QUEUE_IDX, packetsSelected[i].packet);
		}
	}
	return NULL;
}

