#include "mib_queue.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>


static int const queue_num = 0;

void mib_queue_init(struct LockFreeQueue* q)
{
	q->q = malloc(sizeof(struct QueueLFDS));
  mib_queue_lfds_init(q->q);
}

void mib_queue_enqueu(struct LockFreeQueue* q,void* data)
{
  mib_queue_lfds_enqueu(q->q,data);
}

void* mib_queue_deque(struct LockFreeQueue* q)
{
  void* ret = mib_queue_lfds_deque(q->q);
  return ret; 
}

size_t mib_queue_size(struct LockFreeQueue* q)
{
  assert(q != NULL);
  assert(q->q != NULL);

  return mib_queue_lfds_size(q->q);
}

void mib_queue_free(struct LockFreeQueue* q)
{
  mib_queue_lfds_free(q->q);
}

