#include "mib_UPF_sched.h"
#include "mib_priority_queue.h"
#include "mib_time.h"
#include "mib_queue_codel.h"
#include "mib_qfi_queues.h"
#include "mib_upf_queues.h"
#include "mapper.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#define UPF_NUM_PACKETS_PER_TICK 10

static int endThread = 1;
static const int maxNumberPacketsQFI = 1024; 

static void init_mapper(struct mib_mapper* map)
{
	mib_init_mapper(map, UPF_NUM_QUEUES, QFI_NUM_QUEUES);	
	for(int i = 0; i < UPF_NUM_QUEUES; ++i){
		if(i < QFI_NUM_QUEUES){
//			if(i == 0) 
//				mib_set_output_for_input(map, i, 0);
//			else
				mib_set_output_for_input(map, i, i);
//				mib_set_output_for_input(map, i, 0);
		}else{
			mib_set_output_for_input(map, i,  QFI_NUM_QUEUES - 1);
//				mib_set_output_for_input(map, i, 0);
		}
	}	
}

void close_UPF_thread()
{
  endThread = 0;
}

struct packetAndQueue
{
	uint32_t* packet;
	uint32_t queueIdx; 
};

static inline void reset_active_queues(int* arrActiveQueues)
{
	for(int i = 0; i < UPF_NUM_QUEUES; ++i)
	{
		 arrActiveQueues[i] = -1;
	}
}

static uint8_t getActiveUPFQueues(struct UPF_queues* upfQ, int* arrActiveQueues)
{
	uint8_t idx = 0;
	for(int queueIdx = 0; queueIdx < UPF_NUM_QUEUES; ++queueIdx )
	{
		uint32_t queueSize = getUPFBufferStatus(upfQ, queueIdx);
		if(queueSize == 0) continue;

	 	arrActiveQueues[idx] = queueIdx;
		++idx;
	}
	return idx;
}

static inline void printUPFStatus(struct UPF_queues* upfQ, uint8_t numActiveQueues, int* arrActiveQueues)
{
	for(int i = 0 ; i <  numActiveQueues; ++i){
		uint32_t queueIdx = arrActiveQueues[i];
		uint32_t queueSize = getUPFBufferStatus(upfQ, queueIdx);
		printf("UPF queue idx = %d with size = %d at timestamp = %ld \n", queueIdx, queueSize, mib_get_time_us()); 
	}
}

static uint8_t selectUPFPacket(struct UPF_queues* upfQ, uint8_t numActiveQueues, struct packetAndQueue* packetsSelected, uint8_t numberPackets, int* arrActiveQueues)
{
	printUPFStatus(upfQ, numActiveQueues, arrActiveQueues);	

 	uint8_t packetsAlreadySelected = 0;
	while(packetsAlreadySelected < numberPackets && numActiveQueues != 0){
		for(int i = 0; i < numActiveQueues; ++i)
		{
			uint32_t queueIdx = arrActiveQueues[i];
			uint32_t* p = getUPFPacket(upfQ, queueIdx);	
			uint32_t queueSize = getUPFBufferStatus(upfQ, queueIdx);
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

void* thread_UPF_sched(void* threadData)
{
	struct UPF_thread_data* data = (struct UPF_thread_data*)threadData;
	struct packetAndQueue dequePackets[UPF_NUM_PACKETS_PER_TICK];
	int arrActiveQueues[UPF_NUM_QUEUES];
	reset_active_queues(arrActiveQueues);

	struct mib_mapper map;
	init_mapper(&map);

	while(endThread){
		usleep(10500);

		uint8_t numPackets = UPF_NUM_PACKETS_PER_TICK;	
		uint8_t numActQueues = getActiveUPFQueues(data->upfQ, arrActiveQueues);
		uint8_t numPacSel = selectUPFPacket(data->upfQ, numActQueues, dequePackets, numPackets, arrActiveQueues);
		for(int i = 0; i < numPacSel; ++i)
		{
			uint8_t qfiIdx = mib_get_ouput_for_input(&map, dequePackets[i].queueIdx);
			addPacketToQFI(data->qfiQ, qfiIdx, dequePackets[i].packet);
		}
	}
	mib_free_mapper(&map);
	return NULL;
}

