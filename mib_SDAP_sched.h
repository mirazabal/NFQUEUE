#ifndef MIB_SDAP_SCHED_H
#define MIB_SDAP_SCHED_H
#include <stdint.h>

struct SDAP_thread_data
{
  int NFQUEUE_NUM; 
  struct QFI_queues* qfiQ;
  struct DRB_queues* drbQ;
  void (*send_verdict_cb)(uint32_t, uint32_t, uint32_t);
};

void* thread_SDAP_sched(void* threadData);

void close_SDAP_thread();

#endif

