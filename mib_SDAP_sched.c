#include "mib_dq.h"
#include "mib_drb_queues.h"
#include "mib_scenario.h"
#include "mib_SDAP_sched.h"
#include "mib_qfi_queues.h"
#include "mib_realtime_prio.h"
#include "mib_time.h"
#include "mapper.h"
#include "mib_cqi_pacer.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>
#include <pthread.h>

#define SDAP_NUM_PACKETS_PER_TICK 10

static const int forwardPacket = 1;
static int endThread = 1;

#if CQI_PACER || CQI_PACER_ASYNC
static struct DRB_queues* static_drbQ;
#endif
//static const uint64_t maxNumberPacketsDRB = MAX_NUM_PACK_DRB; 

struct packetAndQueue
{
	struct packet_t* packet;
	uint32_t drbIdx;  
};

struct AvailablePacketsPerDRB
{
	int32_t size[DRB_NUM_QUEUES];
	uint8_t arrSize;
};

struct ActiveQFIQueues
{
	int arrIdx[QFI_NUM_QUEUES];
	uint8_t numActiveQueues;
};




static inline void reset_active_queues(int* arrActiveQueues)
{
	for(int i = 0; i < QFI_NUM_QUEUES; ++i)
	{
		arrActiveQueues[i] = -1;
	}
}

static void getActiveQFIQueues(struct QFI_queues* qfiQ, struct ActiveQFIQueues* actQFI)
{
	uint8_t idx = 0;
	for(int queueIdx = 0; queueIdx < QFI_NUM_QUEUES; ++queueIdx )
	{
		uint32_t queueSize = getQFIBufferStatus(qfiQ, queueIdx);
		if(queueSize == 0) continue;

		actQFI->arrIdx[idx] = queueIdx;
		++idx;
	}
	actQFI->numActiveQueues = idx;
}

static inline void printQFIStatus(struct QFI_queues* qfiQ, struct ActiveQFIQueues* availQFI)
{
	for(int i = 0 ; i < availQFI->numActiveQueues; ++i){
		uint32_t queueIdx = availQFI->arrIdx[i];
		uint32_t queueSize = getQFIBufferStatus(qfiQ, queueIdx);
		printf("QFI queue idx = %d with size = %d at timestamp = %ld \n", queueIdx, queueSize, mib_get_time_us()); 
	}
}

static void removeActiveQueue(struct ActiveQFIQueues* qfiQueues, uint8_t idx)
{
	qfiQueues->arrIdx[idx] = qfiQueues->arrIdx[ qfiQueues->numActiveQueues - 1];
	--qfiQueues->numActiveQueues; 
}

struct QFIQueuesInfo
{
	uint32_t num_packets[QFI_NUM_QUEUES];
	uint32_t num_queues_idx[QFI_NUM_QUEUES];
	uint32_t num_queues; 
};

static void addPacketQFIQueuesInfo(struct QFIQueuesInfo* q, uint32_t qfiIdx)
{
	if(q->num_packets[qfiIdx] == 0){
		q->num_queues_idx[q->num_queues] = qfiIdx;
		++q->num_queues;
	}
	++q->num_packets[qfiIdx];
}

static void resetQFIQueuesInfo(struct QFIQueuesInfo* q)
{
	for(int i = 0; i < QFI_NUM_QUEUES; ++i )
	{
		q->num_packets[i] = 0;
		q->num_queues_idx[i] = 0;
	}
	q->num_queues = 0;
}

static uint8_t selectQFIPacket(struct QFI_queues* qfiQ, struct packetAndQueue* packetsSelected, uint8_t numberPackets, struct ActiveQFIQueues* actQFI, struct AvailablePacketsPerDRB* availPacketsDRB, struct mib_mapper* map, struct QFIQueuesInfo* qfiQueuesInfo)
{
	printQFIStatus(qfiQ, actQFI);
	resetQFIQueuesInfo(qfiQueuesInfo);

	uint8_t packetsAlreadySelected = 0;
	while(packetsAlreadySelected < numberPackets && actQFI->numActiveQueues != 0 ){
		for(int i = 0; i < actQFI->numActiveQueues; ++i){
			uint32_t qfiIdx = actQFI->arrIdx[i];
			uint32_t drbIdx = mib_get_ouput_for_input(map,qfiIdx); 
                        printf("drbIdx = %u", drbIdx);
			//assert( availPacketsDRB->size[drbIdx] != -1);
			if(availPacketsDRB->size[drbIdx] <= 0){
				removeActiveQueue(actQFI,i);  
				--i;
				continue;
			}
			struct packet_t *p = getQFIPacket(qfiQ, qfiIdx);	
			if(p == NULL) { // can happen... needs more deep inspection
				continue;
			}

			addPacketQFIQueuesInfo(qfiQueuesInfo,qfiIdx);

			uint32_t queueSize = getQFIBufferStatus(qfiQ, qfiIdx);
			if(queueSize == 0){
				removeActiveQueue(actQFI, i);
				--i;
			}
                        printf("available packets at DRB before -- = %u \n", availPacketsDRB->size[drbIdx]);
			--availPacketsDRB->size[drbIdx];
                        

			packetsSelected[packetsAlreadySelected].packet = p;
			packetsSelected[packetsAlreadySelected].drbIdx = drbIdx;
			++packetsAlreadySelected; 
		}
	}
	return packetsAlreadySelected; 
}

static void resetAvailablePacketsPerDRB(struct AvailablePacketsPerDRB* availablePacketsDRBQueues)
{
	for(uint8_t i = 0; i < DRB_NUM_QUEUES; ++i){
		availablePacketsDRBQueues->size[i] = -1;
	} 
	availablePacketsDRBQueues->arrSize = 0;  
}  

static void getAvailableDRBQueues(struct QFI_queues* qfiQ, struct DRB_queues* drbQ, struct ActiveQFIQueues* activeQFIQueues, struct AvailablePacketsPerDRB* availablePacketsDRBQueues, struct mib_mapper* map)
{
	resetAvailablePacketsPerDRB(availablePacketsDRBQueues);
	//   assert(activeUPFQueues->size != 0);
	uint8_t idx = 0;
	while(idx < activeQFIQueues->numActiveQueues) {
		uint8_t qfiIdx =  activeQFIQueues->arrIdx[idx];
		uint8_t drbIdx = mib_get_ouput_for_input(map, qfiIdx);

		uint32_t availablePac = getDRBAvailablePackets(drbQ,drbIdx);
		if (availablePac == 0){
#if QFI_QUEUE_PACER 
			mib_pacing_saturation_detected(&qfiQ->est[qfiIdx]);
#endif
			removeActiveQueue(activeQFIQueues, idx);
			continue;
		}

		availablePacketsDRBQueues->size[drbIdx] = availablePac;
		++idx;
	}
	availablePacketsDRBQueues->arrSize = idx;
}

/*
#if CQI_PACER || CQI_PACER_ASYNC
void mib_send_data_SDAP(uint32_t channel_max_pac, uint32_t smallest_size, uint32_t DRB_queueIdx)
{
//  const uint32_t QFI_QUEUE = 0;
  mib_cqi_pacer_set( &static_drbQ->pacer[DRB_queueIdx] , channel_max_pac, smallest_size);
}
#endif
*/

void close_SDAP_thread()
{
	endThread = 0;
}

static void init_mapper(struct mib_mapper* map)
{
	mib_init_mapper(map, QFI_NUM_QUEUES, DRB_NUM_QUEUES);	
	for(int i = 0; i < QFI_NUM_QUEUES; ++i){
		mib_set_output_for_input(map, i, 0);
	}	
}

void* thread_SDAP_sched(void *threadData)
{
	mib_set_realtime_priority(sched_get_priority_max(SCHED_RR) - 10);
	struct SDAP_thread_data* data = (struct SDAP_thread_data*)threadData;
	struct packetAndQueue dequePackets[SDAP_NUM_PACKETS_PER_TICK];

	struct AvailablePacketsPerDRB availablePacketsPerDRB;
	struct ActiveQFIQueues activeQFIQueues;
	struct QFIQueuesInfo qfiQueuesInfo;

	struct mib_mapper map;
	init_mapper(&map);

	struct QFI_queues* qfiQ = data->qfiQ;
	struct DRB_queues* drbQ = data->drbQ;
#if CQI_PACER || CQI_PACER_ASYNC
        static_drbQ = data->drbQ;
#endif

	// const uint8_t DRB_QUEUE_IDX = 0;
	while(endThread){
		usleep(SDAP_SCHED_TICK);
		/*
#if DYNAMIC_QUEUE 
uint64_t remainingPac = get_DRB_avail(data->drbQ, DRB_QUEUE_IDX); 
if(remainingPac == 0)
continue;
uint8_t numPackets = remainingPac < SDAP_NUM_PACKETS_PER_TICK + 1 ? remainingPac : SDAP_NUM_PACKETS_PER_TICK;	
#else
uint32_t packetsAtDRB = getDRBBufferStatus(data->drbQ, DRB_QUEUE_IDX); 
if(packetsAtDRB >= getMaxNumberPacketsDRB())
continue;
uint32_t numPackets =  getMaxNumberPacketsDRB() - packetsAtDRB < SDAP_NUM_PACKETS_PER_TICK + 1 ? getMaxNumberPacketsDRB() - packetsAtDRB : SDAP_NUM_PACKETS_PER_TICK;
#endif
*/

		getActiveQFIQueues(qfiQ, &activeQFIQueues);
		getAvailableDRBQueues(qfiQ, drbQ, &activeQFIQueues, &availablePacketsPerDRB, &map);

		if(activeQFIQueues.numActiveQueues == 0 || availablePacketsPerDRB.arrSize == 0){
                  if( activeQFIQueues.numActiveQueues == 0 )  
                    printf("No active QFI queue to schedule in the SDAP entity \n");
                  continue;
                }

		const uint32_t numPackets = SDAP_NUM_PACKETS_PER_TICK; 
		uint8_t numPacSel = selectQFIPacket(data->qfiQ, dequePackets, numPackets, &activeQFIQueues, &availablePacketsPerDRB, &map, &qfiQueuesInfo);

#if QFI_QUEUE_PACER 
		for(int i =0 ; i < qfiQueuesInfo.num_queues; ++i){
			uint32_t qfiIdx = qfiQueuesInfo.num_queues_idx[i];
			uint32_t numPackets = qfiQueuesInfo.num_packets[qfiIdx];
			mib_rate_dequeue(&qfiQ->est[qfiIdx], numPackets, mib_get_time_us());
		}
#endif

		uint32_t pacEnq = 0;
		for(uint32_t i = 0 ; i < numPacSel; ++i)
		{
			uint32_t drbIdx = dequePackets[i].drbIdx;
			struct packet_t *p = dequePackets[i].packet;
			addPacketToDRB(data->drbQ, drbIdx, p);
			++pacEnq;
		}

#if DYNAMIC_QUEUE
		const uint32_t DRB_QUEUE_IDX = 0; 
		//    mib_dq_enqueued(data->drbQ->dq[drbIdx], pacEnq);
		mib_dq_enqueued(data->drbQ->dq[DRB_QUEUE_IDX], pacEnq);
#endif
	}

	mib_free_mapper(&map);
	return NULL;
}

