#include "mib_time.h"
#include "mib_drb_queues.h"
#include "mib_queue.h"
#include "mib_dyn_rlc.h"

#include <stdlib.h>
#include <stdatomic.h>

static _Atomic uint32_t pac_enq = 0;
static _Atomic uint32_t pac_deq = 0;

static size_t getDRBMaxNumberPackets(struct DRB_queues* drbQ, uint8_t idx)
{
#if DYNAMIC_QUEUE 
  return mib_dq_limit(drbQ->dq[idx]);
#elif DYN_RLC
  return mib_dyn_size(drbQ->dyn[idx]);
#else
  return MAX_NUM_PACK_DRB;
#endif
}

void init_DRB_queues(struct DRB_queues* drbQ, void(*verdict)(uint32_t, uint32_t, uint32_t), struct stats_t* stats)
{
  for(uint8_t i = 0; i < DRB_NUM_QUEUES; i++){
#if DRB_QUEUES_CODEL
    drbQ->queues[i] = malloc(sizeof(struct QueueCodel));
    mib_queue_codel_init(drbQ->queues[i],verdict,stats);
#else
    drbQ->queues[i] = malloc(sizeof(struct LockFreeQueue));
    mib_queue_init(drbQ->queues[i]);
#endif

#if DYNAMIC_QUEUE 
    const uint64_t hold_time_us = 100000;
    drbQ->dq[i] = malloc(sizeof(struct mib_dq));
    mib_dq_init(drbQ->dq[i], hold_time_us);
#elif DYN_RLC
    drbQ->dyn[i] = malloc(sizeof(struct mib_dyn_rlc));
    mib_dyn_init(drbQ->dyn[i]);
#endif

  }  
}

void close_DRB_queues(struct DRB_queues* drbQ)
{
  for(uint32_t i = 0; i < DRB_NUM_QUEUES; ++i){
    free(drbQ->queues[i]);
#if DYNAMIC_QUEUE    
    mib_dq_release(drbQ->dq[i]);
#elif DYN_RLC
    mib_dyn_release(drbQ->dyn[i]);
#endif
  }
#if DYNAMIC_QUEUE
  free(drbQ->dq);
#elif DYN_RLC
  free(drbQ->dyn);
#endif
}


#if DYNAMIC_QUEUE
uint64_t get_DRB_avail(struct DRB_queues* drbQ, uint8_t idx)
{
  return mib_dq_avail(drbQ->dq[idx]);
}
#endif

uint32_t getDRBAvailablePackets(struct DRB_queues* drbQ, uint8_t drbIdx)
{
#if DYNAMIC_QUEUE 
  return mib_dq_avail(drbQ->dq[drbIdx]);
//	return mib_dq_limit(drbQ->dq[idx]);
#else
	uint32_t packetsAtDRB = getDRBBufferStatus(drbQ, drbIdx); 
  uint32_t maxPacAllowed = getDRBMaxNumberPackets(drbQ, drbIdx); 
	return maxPacAllowed - packetsAtDRB;
#endif
}

void addPacketToDRB(struct DRB_queues* drbQ, uint8_t queueIdx, struct packet_t* p)
{
  assert(drbQ != NULL);
  assert(drbQ->queues[queueIdx] != NULL);
  assert(queueIdx < DRB_NUM_QUEUES);

  //uint32_t packetsAtDRB = getDRBBufferStatus(drbQ, queueIdx); 
  //p->packets_DRB = packetsAtDRB;
  p->arrival_DRB = mib_get_time_us();

  if(queueIdx == 0)
       pac_enq++;

#if DRB_QUEUES_CODEL
  mib_queue_codel_enqueu(drbQ->queues[queueIdx],p);
#else  
	mib_queue_enqueu(drbQ->queues[queueIdx],p);
#endif
}

struct packet_t* getDRBPacket(struct DRB_queues* drbQ, uint8_t  queueIdx)
{
  assert(drbQ != NULL);
  assert(drbQ->queues[queueIdx] != NULL);
//	mib_dq_dequeued(drbQ->dq,1);

  if(queueIdx == 0)
    pac_deq++;

#if DRB_QUEUES_CODEL
  return mib_queue_codel_deque(drbQ->queues[queueIdx]);
#else	
  return mib_queue_deque(drbQ->queues[queueIdx]);
#endif
}

size_t getDRBBufferStatus(struct DRB_queues* drbQ, uint8_t queueIdx)
{
  assert(drbQ != NULL);
  assert(drbQ->queues[queueIdx] != NULL);

#if DRB_QUEUES_CODEL
  return mib_queue_codel_size(drbQ->queues[queueIdx]);
#else
  return mib_queue_size(drbQ->queues[queueIdx]);
#endif
}



uint32_t getDRB_0_BufferStatus(void)
{
     return pac_enq - pac_deq;
}
