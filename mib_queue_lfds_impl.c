#include "liblfds7.1.1/liblfds711/inc/liblfds711.h"
#include "mib_queue_lfds_impl.h"
#include <stdio.h>

static size_t nb_elements = 0;
static struct lfds711_queue_bmm_element qbmme[512]; // TRD : must be a positive integer power of 2 (2, 4, 8, 16, etc)
static struct lfds711_queue_bmm_state qbmms;

void mib_queue_lfds_init()
{
	lfds711_queue_bmm_init_valid_on_current_logical_core( &qbmms, qbmme, 512, NULL );
}

void mib_queue_lfds_free()
{
	lfds711_queue_bmm_cleanup( &qbmms, NULL );
}

void mib_queue_lfds_enqueu(void* data)
{
//	printf("into enqueu with data value = %u \n",*(unsigned int*)(data)  );
	lfds711_queue_bmm_enqueue( &qbmms, NULL, data);
//	printf("after enqueu \n");
	nb_elements++;
}

void* mib_queue_lfds_deque()
{
	void *data;
  int ret =	lfds711_queue_bmm_dequeue( &qbmms, NULL, &data);
	if(ret == 0) return NULL;

	nb_elements--;
	return data;
}

size_t mib_queue_lfds_size()
{
	return nb_elements;
}

