#include "mib_dq.h"
#include "mib_drb_queues.h"
#include "mib_scenario.h"
#include "mib_SDAP_sched.h"
#include "mib_qfi_queues.h"
#include "mib_time.h"
#include "mapper.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>
#include <pthread.h>

#define SDAP_NUM_PACKETS_PER_TICK 10

static const int forwardPacket = 1;
static int endThread = 1;
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

/*
static uint64_t getMaxNumberPacketsDRB()
{
  return maxNumberPacketsDRB;
}
*/


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


static uint8_t selectQFIPacket(struct QFI_queues* qfiQ, struct packetAndQueue* packetsSelected, uint8_t numberPackets, struct ActiveQFIQueues* actQFI, struct AvailablePacketsPerDRB* availPacketsDRB, struct mib_mapper* map)
{
  printQFIStatus(qfiQ, actQFI);

  uint8_t packetsAlreadySelected = 0;
  while(packetsAlreadySelected < numberPackets && actQFI->numActiveQueues != 0 ){
    for(int i = 0; i < actQFI->numActiveQueues; ++i){
      uint32_t qfiIdx = actQFI->arrIdx[i];
      uint8_t drbIdx = mib_get_ouput_for_input(map,qfiIdx); 
      assert( availPacketsDRB->size[drbIdx] != -1);
      if(availPacketsDRB->size[qfiIdx] == 0){
        removeActiveQueue(actQFI,i);  
        --i;
        continue;
      }
      struct packet_t *p = getQFIPacket(qfiQ, qfiIdx);	
      if(p == NULL) { // can happen... needs more deep inspection
        continue;
      }

      uint32_t queueSize = getQFIBufferStatus(qfiQ, qfiIdx);
      if(queueSize == 0){
        removeActiveQueue(actQFI, i);
        --i;
      }
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

static void getAvailableDRBQueues(struct DRB_queues* drbQ, struct ActiveQFIQueues* activeQFIQueues, struct AvailablePacketsPerDRB* availablePacketsDRBQueues, struct mib_mapper* map)
{
  resetAvailablePacketsPerDRB(availablePacketsDRBQueues);
//   assert(activeUPFQueues->size != 0);
    uint8_t idx = 0;
    while(idx < activeQFIQueues->numActiveQueues) {
      uint8_t drbIdx = mib_get_ouput_for_input(map, activeQFIQueues->arrIdx[idx]);

			uint32_t availablePac = getDRBAvailablePackets(drbQ,drbIdx);
			if (availablePac == 0){
			   removeActiveQueue(activeQFIQueues, idx);
        continue;
			}

//			uint32_t packetsatdrb = getdrbbufferstatus(drbq, drbidx); 
//      uint32_t maxpacketsallowed = getdrbmaxnumberpackets(drbq,drbidx); 


      availablePacketsDRBQueues->size[drbIdx] = availablePac;
			 //	maxPacketsAllowed - packetsAtDRB;
      ++idx;
    }
    availablePacketsDRBQueues->arrSize = idx;
}

void close_SDAP_thread()
{
  endThread = 0;
}

static void set_realtime_priority()
{
  pthread_t this_thread = pthread_self();
  struct sched_param params;
  params.sched_priority = sched_get_priority_max(SCHED_RR) - 10;
  int ret = pthread_setschedparam(this_thread, SCHED_RR, &params);
  if(ret != 0){
    printf("Error while setting the priority of the SDAP thread \n");
  }
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
  set_realtime_priority();
  struct SDAP_thread_data* data = (struct SDAP_thread_data*)threadData;
  struct packetAndQueue dequePackets[SDAP_NUM_PACKETS_PER_TICK];

  struct AvailablePacketsPerDRB availablePacketsPerDRB;
  struct ActiveQFIQueues activeQFIQueues;

  struct mib_mapper map;
  init_mapper(&map);

 // const uint8_t DRB_QUEUE_IDX = 0;
  while(endThread){
    usleep(1000);
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

    getActiveQFIQueues(data->qfiQ, &activeQFIQueues);
    getAvailableDRBQueues(data->drbQ, &activeQFIQueues, &availablePacketsPerDRB, &map);

    if(activeQFIQueues.numActiveQueues == 0 || availablePacketsPerDRB.arrSize == 0) continue;

    const uint32_t numPackets = SDAP_NUM_PACKETS_PER_TICK; 
    uint8_t numPacSel = selectQFIPacket(data->qfiQ, dequePackets, numPackets, &activeQFIQueues, &availablePacketsPerDRB, &map);

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

