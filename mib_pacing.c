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

    est->discrete_time = SDAP_SCHED_TICK ;
    est->discrete_time_next_entity = 10*SDAP_SCHED_TICK;

    est->num_enqueued = 0;
    est->num_enqueued_last = 0;
    est->num_dequeued = 0;

    est->period = 0;
    est->num_ticks = est->discrete_time_next_entity / est->discrete_time;
    est->half_period = est->num_ticks/2;
}

void mib_rate_enqueu(struct mib_rate_estimator* est, uint8_t numPackets)
{
    est->num_enqueued +=  numPackets; 
    printf("Rate enqueued, number of optimal occupancy = %d , num_enqueued = %ld, num_dequeud = %ld number of period = %d \n" , est->opt_occupancy, est->num_enqueued, est->num_dequeued, est->period); 
}

static void start_period_algo(struct mib_rate_estimator* est)
{
    uint64_t num_packets = est->num_enqueued - est->num_enqueued_last;
    uint32_t value = ceil( (double)(num_packets) / (double)(est->num_ticks) );
    printf("Optimal value calculated = %u, with num_packets = %ld and num_ticks = %ld \n", value, num_packets, est->num_ticks);
    est->opt_occupancy = mib_clamp_pack(est, value);
    est->num_enqueued_last = est->num_enqueued;
}

static void calculate_opt_pack(struct mib_rate_estimator* est)
{
    if(est->period == est->half_period){
        est->opt_occupancy = mib_clamp_pack(est, est->opt_occupancy*2);
    }
}

void mib_rate_dequeue(struct mib_rate_estimator* est, uint32_t numPackets, int64_t timestamp)
{
    est->num_dequeued += numPackets;

    printf("Rate dequeued, number of optimal occupancy = %d , num_enqueued = %ld, num_dequeud = %ld with period = %d \n" , est->opt_occupancy, est->num_enqueued, est->num_dequeued , est->period); 
    if(est->saturation == 1){
        est->saturation = 0;
        start_period_algo(est);
        est->period = 0;
    } else if(est->period == est->num_ticks ){
        est->period = 0;
    }	else {
        ++est->period;
        calculate_opt_pack(est);
    }
}

uint32_t mib_get_optim_occupancy(struct mib_rate_estimator* est)
{
    return est->opt_occupancy;
}



