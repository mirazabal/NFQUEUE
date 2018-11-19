#include "mib_queue.h"
#include <stdint.h>
#include <stdio.h>

static int const queue_num = 0;

void mib_queue_init(struct LockFreeQueue* q)
{
    mib_queue_lfds_init(q->q);
}

void mib_queue_enqueu(struct LockFreeQueue* q,void* data)
{
  mib_queue_lfds_enqueu(q->q,data);
}

void* mib_queue_deque(struct LockFreeQueue* q)
{
  void* ret = mib_queue_lfds_deque(q->q);
  if(ret == NULL) 	return ret;
  return ret; 
}

size_t mib_queue_size(struct LockFreeQueue* q)
{
  return  mib_queue_lfds_size(q->q);
}

void mib_queue_free(struct LockFreeQueue* q)
{
  mib_queue_lfds_free(q->q);
}

