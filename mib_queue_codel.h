#ifndef MIB_QUEUE_CODEL
#define MIB_QUEUE_CODEL

#include <stddef.h>
#include <stdint.h>
#include "mib_queue.h"

#define MAX_NUM_PACKETS 4096

struct PacketTimer
{
  int64_t usecs[MAX_NUM_PACKETS];
  int64_t pos;
};
	
struct QueueCodel
{
  uint64_t packets_dropped; //= 0;
  struct PacketTimer pTimer;
  uint32_t first_above_time_; // = 0; //
  uint32_t drop_next_;// = 0;
  uint32_t count_ ; //= 0;
  uint32_t lastcount_; // = 0;
  uint8_t	dropping_ ; //= false;

	struct LockFreeQueue* q;

  uint32_t interval_; // = 200000;// <>  TARGET =  MS2TIME(5);// 5ms TARGET queue delay
  uint32_t target_; // = 10000;
};

void mib_queue_codel_init(struct QueueCodel*, void(*verdict)(uint32_t, uint32_t, uint32_t));

void mib_queue_codel_free(struct QueueCodel* );
void mib_queue_codel_enqueu(struct QueueCodel*,void* data);
void* mib_queue_codel_deque(struct QueueCodel*);
size_t mib_queue_codel_size(struct QueueCodel*);

#endif

