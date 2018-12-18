#ifndef MIB_UPF_QUEUES
#define MIB_UPF_QUEUES

#include <stdint.h>
#include <stddef.h>

#define UPF_NUM_QUEUES 64

struct UPF_queues
{
// struct QueueCodel* queues[UPF_NUM_QUEUES];
  struct LockFreeQueue* queues[UPF_NUM_QUEUES];
  uint8_t sizeQueue;
};

void init_UPF_queues(struct UPF_queues* upfQ, void(*verdict)(uint32_t, uint32_t, uint32_t));

void close_UPF_queues(struct UPF_queues* upfQ);

void addPacketToUPF(struct UPF_queues* upfQ, uint8_t queueIdx, uint32_t id);

uint32_t* getUPFPacket(struct UPF_queues* upfQ, uint8_t  queueIdx);

size_t getUPFBufferStatus(struct UPF_queues* upfQ, uint8_t queueIdx);

#endif

