#include "mib_scenario.h"
#include "mib_UPF_sched.h"
#include "mib_priority_queue.h"
#include "mib_time.h"
#include "mib_queue_codel.h"
#include "mib_qfi_queues.h"
#include "mib_upf_queues.h"
#include "mib_scenario.h"
#include "mib_realtime_prio.h"
#include "mib_pacing.h"
#include "mapper.h"

#include <stdlib.h>
#include <stdio.h>
#include <sched.h>
#include <pthread.h>
#include <unistd.h>

#define UPF_NUM_PACKETS_PER_TICK 10

static int endThread = 1;

static void init_mapper(struct mib_mapper* map)
{
  mib_init_mapper(map, UPF_NUM_QUEUES, QFI_NUM_QUEUES);	
  for(int i = 0; i < UPF_NUM_QUEUES; ++i){
#if FIRST_SCENARIO
    mib_set_output_for_input(map, i, 0);
#else
    if(i < QFI_NUM_QUEUES){
      mib_set_output_for_input(map, i, i);
    }else{
      mib_set_output_for_input(map, i,  QFI_NUM_QUEUES - 1);
    }
#endif
  }	
}

void close_UPF_thread()
{
  endThread = 0;
}

struct packetAndQueue
{
  struct packet_t *packet;
  uint32_t qfiIdx; 
};

struct AvailablePacketsPerQFI
{
  int32_t size[QFI_NUM_QUEUES];
  uint8_t arrSize;
};

struct ActiveUPFQueues
{
  int arrIdx[UPF_NUM_QUEUES];
  uint8_t numActiveQueues;
};

static inline void reset_active_UPF_queues(int* arrActiveQueues)
{
  for(int i = 0; i < UPF_NUM_QUEUES; ++i)
  {
    arrActiveQueues[i] = -1;
  }
}

static void getActiveUPFQueues(struct UPF_queues* upfQ, struct ActiveUPFQueues* activeQueues)
{
  uint8_t idx = 0;
  for(int queueIdx = 0; queueIdx < UPF_NUM_QUEUES; ++queueIdx )
  {
    uint32_t queueSize = getUPFBufferStatus(upfQ, queueIdx);
    if(queueSize == 0) continue;

    activeQueues->arrIdx[idx] = queueIdx;
    ++idx;
  }
  activeQueues->numActiveQueues = idx;
}

static inline void printUPFStatus(struct UPF_queues* upfQ, struct ActiveUPFQueues* availUPF)
{
  for(int i = 0 ; i < availUPF->numActiveQueues; ++i){
    uint32_t queueIdx = availUPF->arrIdx[i];
    uint32_t queueSize = getUPFBufferStatus(upfQ, queueIdx);
    printf("UPF queue idx = %d with size = %d at timestamp = %ld \n", queueIdx, queueSize, mib_get_time_us()); 
  }
}

static void removeActiveQueue(struct ActiveUPFQueues* upfQueues, uint8_t idx)
{
  upfQueues->arrIdx[idx] = upfQueues->arrIdx[ upfQueues->numActiveQueues - 1];
  --upfQueues->numActiveQueues; 
}

static uint8_t selectUPFPacket(struct UPF_queues* upfQ, struct packetAndQueue* packetsSelected, uint8_t numberPackets, struct ActiveUPFQueues* actUPF, struct AvailablePacketsPerQFI* availPacketsQFI, struct mib_mapper* map)
{
  printUPFStatus(upfQ, actUPF);	

  uint8_t packetsAlreadySelected = 0;
  while(packetsAlreadySelected < numberPackets && actUPF->numActiveQueues != 0 ){
    for(int i = 0; i < actUPF->numActiveQueues; ++i){
      uint32_t upfIdx = actUPF->arrIdx[i];
      uint8_t qfiIdx = mib_get_ouput_for_input(map,upfIdx); 
      assert( availPacketsQFI->size[qfiIdx] != -1);
      if(availPacketsQFI->size[qfiIdx] == 0){
        removeActiveQueue(actUPF,i);  
        --i;
        continue;
      }
      struct packet_t *p = getUPFPacket(upfQ, upfIdx);	
      if(p == NULL) { // can happen... needs more deep inspection
        continue;
      }

      uint32_t queueSize = getUPFBufferStatus(upfQ, upfIdx);
      if(queueSize == 0){
        removeActiveQueue(actUPF, i);
        --i;
      }
      --availPacketsQFI->size[qfiIdx];

      packetsSelected[packetsAlreadySelected].packet = p;
      packetsSelected[packetsAlreadySelected].qfiIdx = qfiIdx;
      ++packetsAlreadySelected; 
    }
  }
  return packetsAlreadySelected; 
}

static void resetAvailablePacketsPerQFI(struct AvailablePacketsPerQFI* availablePacketsQFIQueues)
{
  for(uint8_t i = 0; i < QFI_NUM_QUEUES; ++i){
    availablePacketsQFIQueues->size[i] = -1;
  } 
 availablePacketsQFIQueues->arrSize = 0;  
}  

static void getAvailableQFIQueues(struct QFI_queues* qfiQ, struct ActiveUPFQueues* activeUPFQueues, struct AvailablePacketsPerQFI* availablePacketsQFIQueues, struct mib_mapper* map)
{
  resetAvailablePacketsPerQFI(availablePacketsQFIQueues);
//   assert(activeUPFQueues->size != 0);
    uint8_t idx = 0;
    while(idx < activeUPFQueues->numActiveQueues) {
      uint8_t qfiIdx = mib_get_ouput_for_input(map, activeUPFQueues->arrIdx[idx]);
			uint32_t availablePac = getQFIAvailablePackets(qfiQ,qfiIdx);
			if (availablePac == 0){

//      uint32_t packetsAtQFI = getQFIBufferStatus(qfiQ, qfiIdx); 
//      uint32_t maxPacketsAllowed = getQFIMaxNumberPackets(qfiQ,qfiIdx); 

//      if(packetsAtQFI >= maxPacketsAllowed){
				removeActiveQueue(activeUPFQueues, idx);
        continue;
      }
      availablePacketsQFIQueues->size[qfiIdx] = availablePac; 
				
//				maxPacketsAllowed - packetsAtQFI;
      ++idx;
    }
    availablePacketsQFIQueues->arrSize = idx;
}

void* thread_UPF_sched(void* threadData)
{
  mib_set_realtime_priority(sched_get_priority_max(SCHED_RR) - 10);
  struct UPF_thread_data* data = (struct UPF_thread_data*)threadData;
  struct packetAndQueue dequePackets[UPF_NUM_PACKETS_PER_TICK];

  struct ActiveUPFQueues activeUPFQueues;
  struct AvailablePacketsPerQFI availablePacketsPerQFI;

  struct mib_mapper map;
  init_mapper(&map);

  while(endThread){
    usleep(UPF_SCHED_TICK);

    getActiveUPFQueues(data->upfQ, &activeUPFQueues);
    getAvailableQFIQueues(data->qfiQ, &activeUPFQueues, &availablePacketsPerQFI, &map);

    if(activeUPFQueues.numActiveQueues == 0 || availablePacketsPerQFI.arrSize == 0) continue;
 
    const uint8_t numPackets = UPF_NUM_PACKETS_PER_TICK; 
    uint8_t numPacSel = selectUPFPacket(data->upfQ, dequePackets, numPackets, &activeUPFQueues, &availablePacketsPerQFI, &map);

    for(int i = 0; i < numPacSel; ++i)
    {
      uint8_t qfiIdx = dequePackets[i].qfiIdx;
      struct packet_t *p = dequePackets[i].packet;
      addPacketToQFI(data->qfiQ, qfiIdx, p);
      mib_add_packet_QFI_DRB(data->stats);
		
			if(p->UDP_packet == 1){
        printf("UDP Packet with id = %d, inserted into QFI with total packets QFI+DRB = %lu at timestamp = %ld \n", p->idP, mib_get_total_packets_QFI_DRB(data->stats),  mib_get_time_us() ); 
			}
    }
  }
  mib_free_mapper(&map);
  return NULL;
}

