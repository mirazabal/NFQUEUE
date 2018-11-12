#ifndef MIB_QUEUE_CODEL_IMPL
#define MIB_QUEUE_CODEL_IMPL

#include <stddef.h>

void mib_queue_codel_init();
void mib_queue_codel_free();
void mib_queue_codel_enqueu(void* data);
void* mib_queue_codel_deque();
size_t mib_queue_codel_size();

#endif

