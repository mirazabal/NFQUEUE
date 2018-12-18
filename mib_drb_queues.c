#include "mib_drb_queues.h"

#include <stdlib.h>
#include "mib_queue.h"

void init_DRB_queues(struct DRB_queues* drbQ, void(*verdict)(uint32_t, uint32_t, uint32_t))
{
  for(uint8_t i = 0; i < DRB_NUM_QUEUES; i++){
 //   drbQ->queues[i] = malloc(sizeof(struct QueueCodel));
    drbQ->queues[i] = malloc(sizeof(struct LockFreeQueue));
    mib_queue_init(drbQ->queues[i]);
  }  
  drbQ->sizeQueue = DRB_NUM_QUEUES;
}

void close_DRB_queues(struct DRB_queues* drbQ)
{
  for(uint32_t i = 0; i < DRB_NUM_QUEUES; ++i){
    free(drbQ->queues[i]);
  }
}

void addPacketToDRB(struct DRB_queues* drbQ, uint8_t queueIdx, uint32_t* id)
{
  assert(drbQ != NULL);
  assert(queueIdx < DRB_NUM_QUEUES);
  mib_queue_enqueu(drbQ->queues[queueIdx],id);
}

uint32_t* getDRBPacket(struct DRB_queues* drbQ, uint8_t  queueIdx)
{
  assert(drbQ != NULL);
  return mib_queue_deque(drbQ->queues[queueIdx]);
}

size_t getDRBBufferStatus(struct DRB_queues* drbQ, uint8_t queueIdx)
{
  assert(drbQ != NULL);
  assert(drbQ->queues[queueIdx] != NULL);
  return mib_queue_size(drbQ->queues[queueIdx]);
}


