#ifndef MIB_MAC_SCHED
#define MIB_MAC_SCHED

#include <stdint.h>

struct MAC_thread_data
{
  struct DRB_queues* drbQ;
  void (*send_verdict_cb)(uint32_t, uint32_t, uint32_t);
  uint8_t NFQUEUE_NUM; 
};

void* thread_MAC_sched(void* threadData);
void close_MAC_thread();

#endif

