#ifndef MIB_MAC_SCHED
#define MIB_MAC_SCHED

#include <stdint.h>
#include "mib_data_arr.h"

struct MAC_thread_data
{
  struct DRB_queues* drbQ;
  void (*send_verdict_cb)(uint32_t, uint32_t, uint32_t);
	struct stats_t* stats;  
  uint8_t NFQUEUE_NUM; 
#if DYN_RADIO_CHANNEL
  struct data_arr dyn_chan_data;
#endif  
  
};

void* thread_MAC_sched(void* threadData);
void close_MAC_thread();

#endif

