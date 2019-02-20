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

#define NUM_PACKETS_PER_TICK 10

static int endThread = 1;
static const int maxNumberPacketsQFI = 1024; 
static int arrActiveQueues[UPF_NUM_QUEUES];

static void init_mapper(struct mib_mapper* map)
{
	mib_init_mapper(map, UPF_NUM_QUEUES, QFI_NUM_QUEUES);	
	for(int i = 0; i < UPF_NUM_QUEUES; ++i){
		if(i < QFI_NUM_QUEUES){
//			mib_set_output_for_input(map, i, i);
				mib_set_output_for_input(map, i, 0);
		}else{
//			mib_set_output_for_input(map, i,  QFI_NUM_QUEUES - 1);
				mib_set_output_for_input(map, i, 0);
		}
	}	
}

struct PacketAndQueuPos
{
	uint32_t* packet;
	uint8_t queuePos;
};

void close_UPF_thread()
{
  endThread = 0;
}

struct packetAndQueue
{
	uint32_t* packet;
	uint32_t queueIdx; 
};

static uint8_t getActiveUPFQueues(struct UPF_queues* upfQ)
{
	for(int i = 0; i < UPF_NUM_QUEUES; ++i)
	{
	 arrActiveQueues[i] = -1;
	}
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

static uint8_t selectUPFPacket(struct UPF_queues* upfQ, uint8_t activeQueues, struct packetAndQueue* packetsSelected, uint8_t numberPackets)
{
 	uint8_t packetsAlreadySelected = 0;
	while(packetsAlreadySelected < numberPackets && activeQueues != 0){
		for(int i = 0; i < activeQueues; ++i)
		{
			uint32_t queueIdx = arrActiveQueues[i];
			uint32_t* p = getUPFPacket(upfQ, queueIdx);	
			packetsSelected[packetsAlreadySelected].packet = p;
			packetsSelected[packetsAlreadySelected].queueIdx = queueIdx;
			++packetsAlreadySelected; 

			uint32_t queueSize = getUPFBufferStatus(upfQ, queueIdx);
			printf("UPF queue idx = %d with size = %d at timestamp = %ld \n", queueIdx, queueSize, mib_get_time_us()); 
			if(queueSize == 0){
				arrActiveQueues[i] = arrActiveQueues[activeQueues - 1];
				--activeQueues;
			}
		}
	}
	return packetsAlreadySelected; 
}

void* thread_UPF_sched(void* threadData)
{
	struct UPF_thread_data* data = (struct UPF_thread_data*)threadData;
	struct packetAndQueue dequePackets[NUM_PACKETS_PER_TICK];

	struct mib_mapper map;
	init_mapper(&map);

	while(endThread){
		usleep(1000);

		uint8_t actQueues = getActiveUPFQueues(data->upfQ);
		uint8_t numPacSel = selectUPFPacket(data->upfQ, actQueues, dequePackets, NUM_PACKETS_PER_TICK);
		for(int i = 0; i < numPacSel; ++i)
		{
			uint8_t qfiIdx = mib_get_ouput_for_input(&map, dequePackets[i].queueIdx);
			addPacketToQFI(data->qfiQ, qfiIdx, dequePackets[i].packet);
		}
	}
	mib_free_mapper(&map);
	return NULL;
}

