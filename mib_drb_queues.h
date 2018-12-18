#ifndef MIB_DRB_QUEUES
#define MIB_DRB_QUEUES

#define DRB_NUM_QUEUES 11

#include <stddef.h>
#include <stdint.h>
#include "mib_queue_codel.h"

struct DRB_queues
{
  struct LockFreeQueue* queues[DRB_NUM_QUEUES];
//  struct  QueueCodel* queues[DRB_NUM_QUEUES];
  uint8_t sizeQueue;
};

void init_DRB_queues(struct DRB_queues* drbQ, void(*verdict)(uint32_t, uint32_t, uint32_t));

void close_DRB_queues(struct DRB_queues* drbQ);

void addPacketToDRB(struct DRB_queues* drbQ, uint8_t queueIdx, uint32_t *id);

uint32_t* getDRBPacket(struct DRB_queues* drbQ, uint8_t  queueIdx);

size_t getDRBBufferStatus(struct DRB_queues* drbQ, uint8_t queueIdx);

#endif

