#ifndef MIB_PACING
#define MIB_PACING

#include <stdint.h>

struct mib_rate_estimator
{
	uint64_t num_enqueued; 
  uint64_t num_dequeued;
 	uint64_t num_dequeued_last;
	uint32_t min_val_pack; 
	uint32_t max_val_pack; 
	uint32_t saturation; 
	uint32_t opt_occupancy;
	uint32_t opt_occ_set; 
	uint32_t tti; 
	uint32_t num_ticks; 
	int64_t last_time;
};

void mib_init_estimator(struct mib_rate_estimator* est);

void	mib_pacing_saturation_detected(struct mib_rate_estimator* est);

uint32_t mib_get_optim_occupancy(struct mib_rate_estimator* est);

void mib_rate_dequeue(struct mib_rate_estimator* est, uint32_t numPackets, int64_t timestamp);

void mib_rate_enqueu(struct mib_rate_estimator* est, uint8_t numPackets);

#endif
