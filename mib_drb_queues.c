#include "mib_drb_queues.h"

#include <stdlib.h>
#include "mib_queue.h"

void init_DRB_queues(struct DRB_queues* drbQ, void(*verdict)(uint32_t, uint32_t, uint32_t))
{
	const uint64_t hold_time_us = 100000;
  for(uint8_t i = 0; i < DRB_NUM_QUEUES; i++){
//    drbQ->queues[i] = malloc(sizeof(struct QueueCodel));
    drbQ->queues[i] = malloc(sizeof(struct LockFreeQueue));
    mib_queue_init(drbQ->queues[i]);

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

void addPacketToDRB(struct DRB_queues* drbQ, uint8_t queueIdx, uint32_t* id)
{
  assert(drbQ != NULL);
  assert(queueIdx < DRB_NUM_QUEUES);
  mib_queue_enqueu(drbQ->queues[queueIdx],id);
//	mib_dq_enqueued(drbQ->dq,1);
}

uint32_t* getDRBPacket(struct DRB_queues* drbQ, uint8_t  queueIdx)
{
  assert(drbQ != NULL);
//	mib_dq_dequeued(drbQ->dq,1);
  return mib_queue_deque(drbQ->queues[queueIdx]);
}

size_t getDRBBufferStatus(struct DRB_queues* drbQ, uint8_t queueIdx)
{
  assert(drbQ != NULL);
  assert(drbQ->queues[queueIdx] != NULL);
  return mib_queue_size(drbQ->queues[queueIdx]);
}


