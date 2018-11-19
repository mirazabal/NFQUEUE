#ifndef MIB_QUEUE_LFDS_IMPL
#define MIB_QUEUE_LFDS_IMPL

#include <stddef.h>
#include "liblfds7.1.1/liblfds711/inc/liblfds711.h"

struct QueueLFDS
{
  size_t nb_elements = 0;
  struct lfds711_queue_bmm_element qbmme[512]; // TRD : must be a positive integer power of 2 (2, 4, 8, 16, etc)
  struct lfds711_queue_bmm_state qbmms;
};

void mib_queue_lfds_init(struct QueueLFDS* );
void mib_queue_lfds_free(struct QueueLFDS*);
void mib_queue_lfds_enqueu(struct QueueLFDS*, void* data);
void* mib_queue_lfds_deque(struct QueueLFDS*);
size_t mib_queue_lfds_size(struct QueueLFDS*);

#endif

