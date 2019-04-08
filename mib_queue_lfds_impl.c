#include "mib_queue_lfds_impl.h"

#include <stdio.h>
#include <sched.h>

void mib_queue_lfds_init(struct QueueLFDS* q)
{
  assert(q != NULL);
  lfds711_queue_bmm_init_valid_on_current_logical_core( &q->qbmms, q->qbmme, 512, NULL );
}

void mib_queue_lfds_free(struct QueueLFDS* q)
{
  assert(q != NULL);
  lfds711_queue_bmm_cleanup( &q->qbmms, NULL );
}

void mib_queue_lfds_enqueu(struct QueueLFDS* q, void* data)
{
  assert(q != NULL);
  assert(data != NULL);
  //	printf("into enqueu with data value = %u \n",*(unsigned int*)(data)  );
  int ret = lfds711_queue_bmm_enqueue( &q->qbmms, NULL, data);
  if(ret == 1)
    atomic_fetch_add_explicit(&q->nb_elements, 1, memory_order_relaxed); 	
}

void* mib_queue_lfds_deque(struct QueueLFDS* q)
{
  assert(q != NULL);
  if(q->nb_elements < 1) return NULL;

  void *data = NULL;
  int ret = 0;
  while (ret == 0){
    ret = lfds711_queue_bmm_dequeue(&q->qbmms, NULL, &data);
    sched_yield();
  }
  assert(data != NULL);

  //if(q->nb_elements != 0)
  atomic_fetch_sub_explicit(&q->nb_elements, 1, memory_order_relaxed); 	

  return data;
}

size_t mib_queue_lfds_size(struct QueueLFDS* q)
{
  assert(q != NULL);
  return q->nb_elements;
}

