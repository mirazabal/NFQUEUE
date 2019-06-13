#include "mib_scenario.h"
#include "mib_dq.h"
#include "mib_drb_queues.h"
#include "mib_MAC_sched.h"
#include "mib_time.h"
#include "mib_stats.h"

#include <math.h>
#include <pthread.h>
#include <sched.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>
#include <stdio.h>

#define MAC_NUM_PACKETS_PER_TICK 10


#define mib_min(a,b) \
({ __typeof__ (a) _a = (a); \
   __typeof__ (b) _b = (b); \
    _a > _b ? _b : _a; })



static const int forwardPacket = 1;
static int endThread = 1;

struct packetAndQueue
{
  struct packet_t* packet;
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
      struct packet_t* p = getDRBPacket(drbQ, queueIdx);	
      uint32_t queueSize = getDRBBufferStatus(drbQ, queueIdx);
      if(queueSize == 0){
        arrActiveQueues[i] = arrActiveQueues[numActiveQueues - 1];
        --numActiveQueues;
      }

      if(p == NULL) { // can happen... needs more deep inspection
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

#if DYN_RADIO_CHANNEL
static uint32_t num_loop = 0;
static uint8_t get_num_packets(uint32_t num_loop, struct data_arr* arr)
{
  uint32_t index = num_loop/100;
  return arr->data[index];
}
#endif

#if CQI_PACER_ASYNC

static pthread_t pthread_CQI_pacer_async;
static _Atomic int times = 0;
static uint32_t sta_remaining = 0;
static uint32_t sta_channel_buff = 0;

static struct DRB_queues* static_drbQ;

static void* thread_CQI_pacer_async(void* threadData)
{
  time_t t;
  /* Intializes random number generator */
  srand((unsigned) time(&t));

  int random_times = rand() % 10 + 1;
  while(1){

    if(times >= random_times){
      const uint32_t DRB_idx = 0;

      mib_cqi_pacer_set( &static_drbQ->pacer[DRB_idx], sta_channel_buff, sta_remaining);

      random_times = rand() % 10 + 1;
      sta_remaining = UINT32_MAX; // min identity 
      times = 0;
    }
    usleep(5000);
  }
  return NULL;
}

#endif


void* thread_MAC_sched(void* threadData)
{
  struct MAC_thread_data* data = (struct MAC_thread_data*)threadData;   
  struct packetAndQueue dequePackets[MAC_NUM_PACKETS_PER_TICK];
  int arrActiveQueues[DRB_NUM_QUEUES];
  reset_active_queues(arrActiveQueues);


#if CQI_PACER_ASYNC
  static_drbQ = data->drbQ;
  pthread_create(&pthread_CQI_pacer_async , NULL, thread_CQI_pacer_async, NULL);
#endif

  while(endThread){
    usleep(10000);
    sched_yield();

#if DYN_RADIO_CHANNEL
    uint8_t numPackets = get_num_packets(num_loop, &data->dyn_chan_data); 
#else
    uint8_t numPackets = MAC_NUM_PACKETS_PER_TICK;
#endif 

    uint8_t numActQueues = getActiveDRBQueues(data->drbQ,arrActiveQueues);
    uint8_t numPacSel = selectDRBPacket(data->drbQ, numActQueues, dequePackets, numPackets, arrActiveQueues);

    uint64_t pacDeq = 0;
    for(uint8_t i = 0; i < numPacSel; ++i)
    {
      struct packet_t* p = dequePackets[i].packet;
      data->send_verdict_cb(data->NFQUEUE_NUM, (*dequePackets[i].packet).idP,forwardPacket);

      mib_remove_packet_QFI_DRB(data->stats);

      int64_t now = mib_get_time_us();

      if(p->UDP_packet == 1){
        printf("UDP Packet with id = %d, was at the queue UPF = %ld, at QFI = %ld and at DRB = %ld for a total of %ld at timestamp = %ld with packets at QFI = %ld and packets total = %ld \n", p->idP, p->arrival_QFI - p->arrival_UPF, p->arrival_DRB - p->arrival_QFI, now - p->arrival_DRB, now - p->arrival_UPF , now, p->packets_QFI, p->packets_total); 
      } else {
        printf("TCP Packet with id = %d, was at the queue UPF = %ld, at QFI = %ld and at DRB = %ld for a total of %ld at timestamp = %ld with packets at QFI = %ld and packets total = %ld \n", p->idP, p->arrival_QFI - p->arrival_UPF, p->arrival_DRB - p->arrival_QFI, now - p->arrival_DRB, now - p->arrival_UPF , now, p->packets_QFI, p->packets_total); 
      }
#if DYN_RLC
      const uint32_t DRB_QUEUE = 0;
      mib_dyn_sojourn_time( data->drbQ->dyn[DRB_QUEUE], now - p->arrival_DRB);
#endif

      free(dequePackets[i].packet);	
      ++pacDeq;
    }
    printf("Packets deque at MAC scheduler = %lu from a maximum possible of = %u at timestamp = %ld \n", pacDeq, numPackets, mib_get_time_us()); 
    assert(pacDeq < numPackets + 1);
#if DYNAMIC_QUEUE
    mib_dq_dequeued(data->drbQ->dq[0], pacDeq);
#endif
#if DYN_RADIO_CHANNEL
    ++num_loop;
#endif    

#if CQI_PACER
    const uint32_t DRB_idx = 0;
    uint32_t remaining = mib_queue_size(data->drbQ->queues[DRB_idx]);

    mib_cqi_pacer_set(&data->drbQ->pacer[DRB_idx], numPackets,  remaining );

#elif CQI_PACER_ASYNC

    const uint32_t DRB_idx = 0;
    uint32_t remaining = mib_queue_size(data->drbQ->queues[DRB_idx]);

    sta_channel_buff = numPackets; 
    sta_remaining = mib_min(sta_remaining,remaining);
    times++;

#endif
  }
  return NULL;
}

