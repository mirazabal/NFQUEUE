#ifndef MIB_QUEUE
#define MIB_QUEUE
#include <stdint.h>
#include <stddef.h>

void mib_queue_init(void(*verdict)(uint32_t, uint32_t, uint32_t) );
void mib_queue_free();
void mib_queue_enqueu(void* data);
void* mib_queue_deque();
size_t mib_queue_size();

#endif

