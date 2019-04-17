#include "mib_upf_queues.h"
#include "mib_queue.h"
#include "mib_queue_codel.h"
#include "mib_time.h"

#include <stdlib.h>
#include <stdio.h>

void init_UPF_queues(struct UPF_queues* upfQ, void(*verdict)(uint32_t, uint32_t, uint32_t), struct stats_t* stats)
{
  assert(upfQ != NULL);
  for(uint32_t i = 0; i < UPF_NUM_QUEUES; ++i){
#if UPF_QUEUES_CODEL 
    upfQ->queues[i] = malloc(sizeof(struct QueueCodel));
    mib_queue_codel_init(upfQ->queues[i],verdict, stats); 
#else
  upfQ->queues[i] = malloc(sizeof(struct LockFreeQueue));
  mib_queue_init(upfQ->queues[i]); 
#endif
  }
  upfQ->sizeQueue = UPF_NUM_QUEUES;
}

void close_UPF_queues(struct UPF_queues* upfQ)
{
  for(uint32_t i = 0; i < UPF_NUM_QUEUES; ++i){
    free(upfQ->queues[i]);
  }
}

void addPacketToUPF(struct UPF_queues* upfQ, uint8_t queueIdx, uint32_t id)
{
  assert(upfQ != NULL);
  assert(upfQ->queues[queueIdx] != NULL);
  assert(queueIdx < UPF_NUM_QUEUES);

  // first step in the QoS model. The data has to be freed if it is discarded or if it gets into the last step if the packet is accepted

  struct packet_t *p = malloc(sizeof(struct packet_t));	
  p->idP = id;
  p->arrival_UPF = mib_get_time_us();
  if(queueIdx == 0){
    p->UDP_packet = 1;
		uint32_t bufferSize = getUPFBufferStatus(upfQ, queueIdx); 
    printf("UDP Packet with id = %d, with a UPF queue size = %d, inserted into UPF at timestamp = %ld \n", id, bufferSize, p->arrival_UPF); 
	}
  else
    p->UDP_packet = 0;

  //  uint32_t* idP = malloc(sizeof(uint32_t));
  //  *idP = id;
#if UPF_QUEUES_CODEL 
  //mib_queue_codel_enqueu(upfQ->queues[queueIdx],idP);
  mib_queue_codel_enqueu(upfQ->queues[queueIdx],p);
#else  
  //mib_queue_enqueu(upfQ->queues[queueIdx],idP);
  mib_queue_enqueu(upfQ->queues[queueIdx],p);
#endif
}

//uint32_t* getUPFPacket(struct UPF_queues* upfQ, uint8_t  queueIdx)
struct packet_t* getUPFPacket(struct UPF_queues* upfQ, uint8_t  queueIdx)
{
  assert(upfQ != NULL);
  assert(upfQ->queues[queueIdx] != NULL);
#if UPF_QUEUES_CODEL 
  return mib_queue_codel_deque( upfQ->queues[queueIdx]);
#else  
	return mib_queue_deque( upfQ->queues[queueIdx]);
#endif
}

size_t getUPFBufferStatus(struct UPF_queues* upfQ, uint8_t queueIdx)
{
  assert(upfQ != NULL);
  assert(upfQ->queues[queueIdx] != NULL);

#if UPF_QUEUES_CODEL 
  return mib_queue_codel_size(upfQ->queues[queueIdx]);
#else  
	return mib_queue_size(upfQ->queues[queueIdx]);
#endif
}


