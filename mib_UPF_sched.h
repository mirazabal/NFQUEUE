#ifndef MIB_UPF_SCHED_H
#define MIB_UPF_SCHED_H

#include "mib_qfi_queues.h"
#include "mib_stats.h"

#include <stdint.h>

struct UPF_thread_data
{
  struct UPF_queues* upfQ;
  struct QFI_queues* qfiQ; 
  void (*send_verdict_cb)(uint32_t, uint32_t, uint32_t);
	struct stats_t* stats;  
};

void* thread_UPF_sched(void* threadData);
void close_UPF_thread();

#endif

