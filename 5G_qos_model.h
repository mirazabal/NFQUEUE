#ifndef MIB_SCHEDULER
#define MIB_SCHEDULER
#include <stddef.h>
#include <stdint.h>

void init_5G_qos_model(void(*send_verdict)(uint32_t,uint32_t,uint32_t));
void add_packet_5G_qos_model(uint32_t,uint32_t,uint32_t);
void close_5G_qos_model();

#endif

