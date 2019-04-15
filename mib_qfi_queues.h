#ifndef MIB_QFI_QUEUES
#define MIB_QFI_QUEUES

#include "5G_qos_model.h"
#include "mib_scenario.h"
#include "mib_stats.h"

#include <stdint.h>
#include <stddef.h>
#include "mib_stats.h"

#define QFI_NUM_QUEUES 4


struct QFI_queues
{
#if QFI_QUEUES_CODEL
 struct QueueCodel* queues[QFI_NUM_QUEUES];
#else
 struct LockFreeQueue* queues[QFI_NUM_QUEUES];
#endif
 size_t maxNumberPackets[QFI_NUM_QUEUES];
};

void init_QFI_queues(struct QFI_queues* qfiQ, void(*verdict)(uint32_t, uint32_t, uint32_t), struct stats_t*);

void close_QFI_queues(struct QFI_queues* qfiQ);

void addPacketToQFI(struct QFI_queues* qfiQ, uint8_t  queueIdx, struct packet_t* p);

struct packet_t* getQFIPacket(struct  QFI_queues* qfiQ, uint8_t queueIdx);

size_t getQFIBufferStatus(struct QFI_queues* qfiQ, uint8_t queueIdx);

size_t getQFIMaxNumberPackets(struct QFI_queues* qfiQ, uint8_t queueIdx);

void setQFIMaxNumberPackets(struct QFI_queues* qfiQ, uint8_t queueIdx, size_t maxNumPackets);


#endif

