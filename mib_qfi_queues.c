
#include "mib_qfi_queues.h"
#include "mib_queue.h"
#include "mib_queue_codel.h"
#include "mib_time.h"

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

void init_QFI_queues(struct QFI_queues* qfiQ, void(*verdict)(uint32_t, uint32_t, uint32_t), struct stats_t* stats)
{
  assert(qfiQ != NULL);
  for(uint32_t i = 0; i < QFI_NUM_QUEUES; ++i){
#if QFI_QUEUES_CODEL
    qfiQ->queues[i] = malloc(sizeof(struct QueueCodel));
    qfiQ->maxNumberPackets[i] = MAX_NUM_PACK_QFI; 
    mib_queue_codel_init(qfiQ->queues[i],verdict,stats); 
#else
    qfiQ->queues[i] = malloc(sizeof(struct LockFreeQueue));
    qfiQ->maxNumberPackets[i] = MAX_NUM_PACK_QFI; 
    mib_queue_init(qfiQ->queues[i]); 
#endif
  }
}

void close_QFI_queues(struct QFI_queues* qfiQ)
{
  for(uint32_t i = 0; i < QFI_NUM_QUEUES; ++i){
    free(qfiQ->queues[i]);
  }
}

void addPacketToQFI(struct QFI_queues* qfiQ, uint8_t queueIdx, struct packet_t* p)
{
  assert(qfiQ != NULL);
  assert(queueIdx < QFI_NUM_QUEUES);
//	printf("Packet added to qfi index = %ld", queueIdx);

  uint32_t packetsAtQFI = getQFIBufferStatus(qfiQ, queueIdx); 
  p->packets_QFI = packetsAtQFI;
  p->arrival_QFI = mib_get_time_us();

#if QFI_QUEUES_CODEL
  mib_queue_codel_enqueu(qfiQ->queues[queueIdx],p);
#else
  mib_queue_enqueu(qfiQ->queues[queueIdx],p);
#endif
}

struct packet_t* getQFIPacket(struct  QFI_queues* qfiQ, uint8_t queueIdx)
{
  assert(qfiQ != NULL);
  assert(queueIdx < QFI_NUM_QUEUES);
#if QFI_QUEUES_CODEL
  return mib_queue_codel_deque( qfiQ->queues[queueIdx]);
#else
  return mib_queue_deque( qfiQ->queues[queueIdx]);
#endif	
}

size_t getQFIBufferStatus(struct QFI_queues* qfiQ, uint8_t queueIdx)
{
  assert(qfiQ != NULL);
  assert(qfiQ->queues[queueIdx] != NULL);
#if QFI_QUEUES_CODEL
  return mib_queue_codel_size(qfiQ->queues[queueIdx]);
#else
  return mib_queue_size(qfiQ->queues[queueIdx]);
#endif
}

size_t getQFIMaxNumberPackets(struct QFI_queues* qfiQ, uint8_t queueIdx)
{
  return qfiQ->maxNumberPackets[queueIdx];
}

void setQFIMaxNumberPackets(struct QFI_queues* qfiQ, uint8_t queueIdx, size_t numPackets)
{
  qfiQ->maxNumberPackets[queueIdx] = numPackets;
}


