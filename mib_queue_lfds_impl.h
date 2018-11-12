#ifndef MIB_QUEUE_LFDS_IMPL
#define MIB_QUEUE_LFDS_IMPL

#include <stddef.h>

void mib_queue_lfds_init();
void mib_queue_lfds_free();
void mib_queue_lfds_enqueu(void* data);
void* mib_queue_lfds_deque();
size_t mib_queue_lfds_size();

#endif

