#include "mib_upf_queues.h"
#include "mib_queue.h"
#include "mib_queue_codel.h"

#include <stdlib.h>

void init_UPF_queues(struct UPF_queues* upfQ, void(*verdict)(uint32_t, uint32_t, uint32_t))
{
  assert(upfQ != NULL);
  for(uint32_t i = 0; i < UPF_NUM_QUEUES; ++i){
    upfQ->queues[i] = malloc(sizeof(struct LockFreeQueue));
//    upfQ->queues[i] = malloc(sizeof(struct QueueCodel));
    mib_queue_init(upfQ->queues[i]); 
  }
  upfQ->sizeQueue = UPF_NUM_QUEUES;
}

void close_UPF_queues(struct UPF_queues* upfQ)
{
  for(uint32_t i = 0; i < UPF_NUM_QUEUES; ++i){
    free(upfQ->queues[i]);
  }
}

void addPacketToUPF(struct UPF_queues* upfQ, uint8_t queueIdx, uint32_t id)
{
  assert(upfQ != NULL);
  assert(queueIdx < UPF_NUM_QUEUES);

  // first step in the QoS model. The data has to be freed if it is discarded or if it gets into the last step if the packet is accepted
  uint32_t* idP = malloc(sizeof(uint32_t));
  *idP = id;
  mib_queue_enqueu(upfQ->queues[queueIdx],idP);
}

uint32_t* getUPFPacket(struct UPF_queues* upfQ, uint8_t  queueIdx)
{
  assert(upfQ != NULL);
  return mib_queue_deque( upfQ->queues[queueIdx]);
}

size_t getUPFBufferStatus(struct UPF_queues* upfQ, uint8_t queueIdx)
{
  assert(upfQ != NULL);
  assert(upfQ->queues[queueIdx] != NULL);
  return mib_queue_size(upfQ->queues[queueIdx]);
}


