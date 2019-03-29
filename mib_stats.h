#ifndef MIB_STATS
#define MIB_STATS

#include <stdint.h>

struct stats_t{
	uint64_t data_enq;
	uint64_t data_deq;

};



void mib_remove_packet_QFI_DRB(struct stats_t* s);
void mib_add_packet_QFI_DRB(struct stats_t* s);
uint64_t mib_get_total_packets_QFI_DRB(struct stats_t* s);

#endif
