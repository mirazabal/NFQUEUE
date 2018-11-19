#ifndef MIB_SCHEDULER
#define MIB_SCHEDULER
#include <stddef.h>
#include <stdint.h>

void init_sched(void(*send_verdict)(uint32_t,uint32_t,uint32_t));
void add_packet_sched(uint32_t,uint32_t,uint32_t);
void close_sched();

#endif



