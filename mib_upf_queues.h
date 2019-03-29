#ifndef MIB_UPF_QUEUES
#define MIB_UPF_QUEUES

#include "5G_qos_model.h"
#include "mib_scenario.h"

#include <stdint.h>
#include <stddef.h>

#define UPF_NUM_QUEUES 64


struct UPF_queues
{
#if UPF_QUEUES_CODEL 
 struct QueueCodel* queues[UPF_NUM_QUEUES];
#else
  struct LockFreeQueue* queues[UPF_NUM_QUEUES];
#endif
  uint8_t sizeQueue;
};

void init_UPF_queues(struct UPF_queues* upfQ, void(*verdict)(uint32_t, uint32_t, uint32_t));

void close_UPF_queues(struct UPF_queues* upfQ);

void addPacketToUPF(struct UPF_queues* upfQ, uint8_t queueIdx, uint32_t id);

struct packet_t* getUPFPacket(struct UPF_queues* upfQ, uint8_t  queueIdx);

size_t getUPFBufferStatus(struct UPF_queues* upfQ, uint8_t queueIdx);

#endif

