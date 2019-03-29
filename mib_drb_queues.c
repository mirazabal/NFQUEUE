#include "mib_time.h"
#include "mib_drb_queues.h"

#include <stdlib.h>
#include "mib_queue.h"

void init_DRB_queues(struct DRB_queues* drbQ, void(*verdict)(uint32_t, uint32_t, uint32_t))
{
	const uint64_t hold_time_us = 100000;
  for(uint8_t i = 0; i < DRB_NUM_QUEUES; i++){
#if DRB_QUEUES_CODEL
    drbQ->queues[i] = malloc(sizeof(struct QueueCodel));
    mib_queue_codel_init(drbQ->queues[i],verdict);
#else
    drbQ->queues[i] = malloc(sizeof(struct LockFreeQueue));
    mib_queue_init(drbQ->queues[i]);
#endif

		drbQ->dq[i] = malloc(sizeof(struct mib_dq));
		mib_dq_init(drbQ->dq[i], hold_time_us);
  }  
}

void close_DRB_queues(struct DRB_queues* drbQ)
{
  for(uint32_t i = 0; i < DRB_NUM_QUEUES; ++i){
    free(drbQ->queues[i]);
  }
	free(drbQ->dq);
}

uint64_t get_DRB_avail(struct DRB_queues* drbQ, uint8_t idx)
{
	return mib_dq_avail(drbQ->dq[idx]);
}

void addPacketToDRB(struct DRB_queues* drbQ, uint8_t queueIdx, struct packet_t* p)
{
  assert(drbQ != NULL);
  assert(drbQ->queues[queueIdx] != NULL);
  assert(queueIdx < DRB_NUM_QUEUES);

	p->arrival_DRB = mib_get_time_us();
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


