#include "mib_queue_lfds_impl.h"
#include <stdio.h>

void mib_queue_lfds_init(struct QueueLFDS* q)
{
	lfds711_queue_bmm_init_valid_on_current_logical_core( &q->qbmms, q->qbmme, 512, NULL );
}

void mib_queue_lfds_free(struct QueueLFDS* q)
{
	lfds711_queue_bmm_cleanup( &q->qbmms, NULL );
}

void mib_queue_lfds_enqueu(struct QueueLFDS* q, void* data)
{
//	printf("into enqueu with data value = %u \n",*(unsigned int*)(data)  );
	lfds711_queue_bmm_enqueue( &q->qbmms, NULL, data);
	q->nb_elements++;
}

void* mib_queue_lfds_deque(struct QueueLFDS* q)
{
	void *data;
  int ret =	lfds711_queue_bmm_dequeue( &q->qbmms, NULL, &data);
	if(ret == 0) return NULL;

	q->nb_elements--;
	return data;
}

size_t mib_queue_lfds_size(struct QueueLFDS* q)
{
	return q->nb_elements;
}

