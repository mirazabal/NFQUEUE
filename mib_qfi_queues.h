#ifndef MIB_QFI_QUEUES
#define MIB_QFI_QUEUES

#include <stdint.h>
#include <stddef.h>

#define QFI_NUM_QUEUES 8

struct QFI_queues
{
// struct LockFreeQueue* queues[QFI_NUM_QUEUES];
 struct QueueCodel* queues[QFI_NUM_QUEUES];
};

void init_QFI_queues(struct QFI_queues* qfiQ, void(*verdict)(uint32_t, uint32_t, uint32_t));

void close_QFI_queues(struct QFI_queues* qfiQ);

void addPacketToQFI(struct QFI_queues* qfiQ, uint8_t  queueIdx, uint32_t* id);

uint32_t* getQFIPacket(struct  QFI_queues* qfiQ, uint8_t queueIdx);

size_t getQFIBufferStatus(struct QFI_queues* qfiQ, uint8_t queueIdx);

#endif

