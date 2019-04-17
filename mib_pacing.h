#ifndef MIB_PACING
#define MIB_PACING

#include <stdint.h>

struct mib_rate_estimator
{
//	uint64_t num_enqueued_last;
	uint64_t num_enqueued; 
  uint64_t num_dequeued;
 	uint64_t num_enqueued_last;

//	int64_t slack_start_time;
	uint32_t period;
	uint32_t half_period;


//	uint32_t min_val_time;
//	uint32_t max_val_time; 

	uint32_t min_val_pack; 
	uint32_t max_val_pack; 

	uint32_t saturation; 

	uint32_t opt_occupancy;
	uint32_t discrete_time_next_entity;

	uint32_t num_ticks;
	uint32_t discrete_time;
};

void mib_init_estimator(struct mib_rate_estimator* est);


void	mib_pacing_saturation_detected(struct mib_rate_estimator* est);

//void mib_packets_added(struct mib_rate_estimator* est, uint8_t numPackets, int64_t timestamp);

uint32_t mib_get_optim_occupancy(struct mib_rate_estimator* est);

void mib_rate_dequeue(struct mib_rate_estimator* est, uint32_t numPackets, int64_t timestamp);

void mib_rate_enqueu(struct mib_rate_estimator* est, uint8_t numPackets);

#endif
