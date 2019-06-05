#ifndef MIB_DRB_QUEUES
#define MIB_DRB_QUEUES

#include "mib_queue_codel.h"
#include "mib_dq.h"
#include "5G_qos_model.h"
#include "mib_stats.h"
#include "mib_scenario.h"

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

#if DYNAMIC_QUEUE 
  struct mib_dq* dq[DRB_NUM_QUEUES]; 
  uint8_t sizeQueue;
#elif DYN_RLC
  struct mib_dyn_rlc* dyn[DRB_NUM_QUEUES]; 
#endif

};

void init_DRB_queues(struct DRB_queues* drbQ, void(*verdict)(uint32_t, uint32_t, uint32_t), struct stats_t* stats );

void close_DRB_queues(struct DRB_queues* drbQ);

void addPacketToDRB(struct DRB_queues* drbQ, uint8_t queueIdx, struct packet_t* p);

struct packet_t* getDRBPacket(struct DRB_queues* drbQ, uint8_t  queueIdx);

size_t getDRBBufferStatus(struct DRB_queues* drbQ, uint8_t queueIdx);

#if DYNAMIC_QUEUE
uint64_t get_DRB_avail(struct DRB_queues* drbQ, uint8_t idx);
#endif

uint32_t getDRBAvailablePackets(struct DRB_queues* drbQ, uint8_t drbIdx);

// delete this function ASAP
uint32_t getDRB_0_BufferStatus(void);

#endif

