#include "mib_stats.h"

#include <assert.h>
#include <stddef.h>

void mib_remove_packet_QFI_DRB(struct stats_t* s)
{
  assert(s != NULL);
  atomic_fetch_add_explicit(&s->data_deq, 1, memory_order_relaxed); 	
//  ++s->data_deq;

};

void mib_add_packet_QFI_DRB(struct stats_t* s)
{
  assert(s != NULL);
  // packets are just added from UPF, while packets are deque from CoDel + MAC sched
  ++s->data_enq;
}
	
uint64_t mib_get_total_packets_QFI_DRB(struct stats_t* s)
{
  assert(s != NULL);
  assert(s->data_enq >= s->data_deq);
  return s->data_enq - s->data_deq;
}

