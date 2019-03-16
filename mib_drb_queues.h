#ifndef MIB_DRB_QUEUES
#define MIB_DRB_QUEUES

#include <stddef.h>
#include <stdint.h>
#include "mib_queue_codel.h"
#include "mib_dq.h"

#define DRB_NUM_QUEUES 11

struct DRB_queues
{
  struct LockFreeQueue* queues[DRB_NUM_QUEUES];
//  struct  QueueCodel* queues[DRB_NUM_QUEUES];
	struct mib_dq* dq[DRB_NUM_QUEUES]; 
  uint8_t sizeQueue;
};

void init_DRB_queues(struct DRB_queues* drbQ, void(*verdict)(uint32_t, uint32_t, uint32_t));

void close_DRB_queues(struct DRB_queues* drbQ);

void addPacketToDRB(struct DRB_queues* drbQ, uint8_t queueIdx, uint32_t *id);

uint32_t* getDRBPacket(struct DRB_queues* drbQ, uint8_t  queueIdx);

size_t getDRBBufferStatus(struct DRB_queues* drbQ, uint8_t queueIdx);

uint64_t get_DRB_avail(struct DRB_queues* drbQ, uint8_t idx);

#endif

