#ifndef MIB_QUEUE_CODEL
#define MIB_QUEUE_CODEL

#include <stddef.h>
#include <stdint.h>

void mib_queue_codel_init( void(*verdict)(uint32_t, uint32_t, uint32_t));

void mib_queue_codel_free();
void mib_queue_codel_enqueu(void* data);
void* mib_queue_codel_deque();
size_t mib_queue_codel_size();

#endif

