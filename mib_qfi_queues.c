#include <assert.h>
#include <stdlib.h>
#include "mib_qfi_queues.h"
#include "mib_queue.h"
#include "mib_queue_codel.h"

void init_QFI_queues(struct QFI_queues* qfiQ, void(*verdict)(uint32_t, uint32_t, uint32_t))
{
  assert(qfiQ != NULL);
  for(uint32_t i = 0; i < QFI_NUM_QUEUES; ++i){
//    qfiQ->queues[i] = malloc(sizeof(struct LockFreeQueue));
    qfiQ->queues[i] = malloc(sizeof(struct QueueCodel));

    mib_queue_codel_init(qfiQ->queues[i],verdict); 
  }
}

void close_QFI_queues(struct QFI_queues* qfiQ)
{
  for(uint32_t i = 0; i < QFI_NUM_QUEUES; ++i){
    free(qfiQ->queues[i]);
  }
}

void addPacketToQFI(struct QFI_queues* qfiQ, uint8_t queueIdx, uint32_t* idP)
{
  assert(qfiQ != NULL);
  assert(queueIdx < QFI_NUM_QUEUES);
  mib_queue_codel_enqueu(qfiQ->queues[queueIdx],idP);
}

uint32_t* getQFIPacket(struct  QFI_queues* qfiQ, uint8_t queueIdx)
{
  assert(qfiQ != NULL);
  assert(queueIdx < QFI_NUM_QUEUES);
  return mib_queue_codel_deque( qfiQ->queues[queueIdx]);
}

size_t getQFIBufferStatus(struct QFI_queues* qfiQ, uint8_t queueIdx)
{
  assert(qfiQ != NULL);
  assert(qfiQ->queues[queueIdx] != NULL);
  return mib_queue_codel_size( qfiQ->queues[queueIdx]);
}

