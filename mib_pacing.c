#include "mib_pacing.h"
#include "mib_time.h"
#include "mib_scenario.h" 

#include <math.h>
#include <stdio.h>

void mib_pacing_saturation_detected(struct mib_rate_estimator* est)
{
	est->saturation = 1;
	printf("Saturation detected, num of optimal occupancy = %d , num_enqueued = %ld, num_dequeud = %ld \n" 
			, est->opt_occupancy, est->num_enqueued, est->num_dequeued); 
}

static inline uint32_t mib_clamp_pack(struct mib_rate_estimator* est, uint32_t value)
{
	return value < est->min_val_pack ? est->min_val_pack : value > est->max_val_pack ? est->max_val_pack : value; 
}

void mib_init_estimator(struct mib_rate_estimator* est)
{
	est->min_val_pack = 1; 
	est->max_val_pack = 1024; 
	est->saturation = 0;
	est->opt_occupancy = 1024;
	est->opt_occ_set = 0;
	est->num_enqueued = 0;
	est->num_dequeued = 0;
	est->num_dequeued_last = 0;
	est->tti = 10000;
	est->num_ticks = 0; 
	est->last_time = mib_get_time_us();
}

void mib_rate_enqueu(struct mib_rate_estimator* est, uint8_t numPackets)
{
	est->num_enqueued +=  numPackets; 
	printf("Rate enqueued, number of optimal occupancy = %d , num_enqueued = %ld, num_dequeud = %ld \n" , est->opt_occupancy, est->num_enqueued, est->num_dequeued); 
}

static void calc_opt_occ(struct mib_rate_estimator* est)
{
	uint64_t num_packets = est->num_dequeued - est->num_dequeued_last;
	uint32_t value = ceil( (double)(num_packets) / (double)(est->num_ticks) );
	printf("Optimal value calculated = %u, with num_packets = %ld and num_ticks = %u \n", value, num_packets, est->num_ticks);
	est->opt_occupancy = mib_clamp_pack(est, value);
}

static void increase_opt_val(struct mib_rate_estimator* est, double factor)
{
	est->opt_occ_set = 1;
	est->opt_occupancy = mib_clamp_pack(est, est->opt_occupancy*factor);
}

static void reset_values(struct mib_rate_estimator* est, int64_t now)
{
	est->num_dequeued_last = est->num_dequeued;
	est->num_ticks = 0;
	est->opt_occ_set = 0;
	est->last_time = now;
}

void mib_rate_dequeue(struct mib_rate_estimator* est, uint32_t numPackets, int64_t now)
{
	printf("Rate dequeued, number of optimal occupancy = %d , num_enqueued = %ld, num_dequeud = %ld \n" , est->opt_occupancy, est->num_enqueued, est->num_dequeued); 
	if(est->saturation == 1){
		est->saturation = 0;
		calc_opt_occ(est);
		reset_values(est,now);
	} else if(est->last_time + est->tti > now){
		reset_values(est,now);
	}
	est->num_dequeued += numPackets;
}

uint32_t mib_get_optim_occupancy(struct mib_rate_estimator* est)
{
	++est->num_ticks;
	int64_t now = mib_get_time_us();
	if(est->last_time + est->tti/2 > now && est->opt_occ_set == 0)
		increase_opt_val(est,2.0);    
	return est->opt_occupancy;
}

