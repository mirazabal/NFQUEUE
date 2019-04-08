#ifndef MIB_DRB_QUEUES
#define MIB_DRB_QUEUES

#include "mib_queue_codel.h"
#include "mib_dq.h"
#include "5G_qos_model.h"
#include "mib_stats.h"

#include <stddef.h>
#include <stdint.h>
#define DRB_NUM_QUEUES 11

struct DRB_queues
{

#if DRB_QUEUES_CODEL 
  struct  QueueCodel* queues[DRB_NUM_QUEUES];
#else  
  struct LockFreeQueue* queues[DRB_NUM_QUEUES];
#endif

  struct mib_dq* dq[DRB_NUM_QUEUES]; 
  uint8_t sizeQueue;
};

void init_DRB_queues(struct DRB_queues* drbQ, void(*verdict)(uint32_t, uint32_t, uint32_t), struct stats_t* stats );

void close_DRB_queues(struct DRB_queues* drbQ);

void addPacketToDRB(struct DRB_queues* drbQ, uint8_t queueIdx, struct packet_t* p);

struct packet_t* getDRBPacket(struct DRB_queues* drbQ, uint8_t  queueIdx);

size_t getDRBBufferStatus(struct DRB_queues* drbQ, uint8_t queueIdx);

uint64_t get_DRB_avail(struct DRB_queues* drbQ, uint8_t idx);

size_t getDRBMaxNumberPackets(struct DRB_queues* drbQ, uint8_t queueIdx);


#endif

