#ifndef MIB_SCHEDULER
#define MIB_SCHEDULER
#include <stddef.h>
#include <stdint.h>

struct packet_t
{
  int64_t arrival_UPF;
  int64_t arrival_QFI;
  uint64_t packets_QFI;
  int64_t arrival_DRB;
  uint64_t packets_total;
  uint32_t idP;
  uint8_t UDP_packet;
};


void init_5G_qos_model(void(*send_verdict)(uint32_t,uint32_t,uint32_t));
void add_packet_5G_qos_model(uint32_t,uint32_t,uint32_t);
void close_5G_qos_model();

#endif

