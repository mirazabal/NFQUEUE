#ifndef MIB_QUEUE
#define MIB_QUEUE
#include <stdint.h>
#include <stddef.h>
#include "mib_queue_lfds_impl.h"

struct LockFreeQueue
{
  struct QueueLFDS* q;
};

void mib_queue_init(struct LockFreeQueue*);
void mib_queue_free(struct LockFreeQueue*);
void mib_queue_enqueu(struct LockFreeQueue*, void* data);
void* mib_queue_deque(struct LockFreeQueue* );
size_t mib_queue_size(struct LockFreeQueue* );

#endif

