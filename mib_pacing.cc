#include "mib_pacing.h"
#include "mib_time.h"

static inline void saturation_detected(struct mib_rate_estimator* est)
{
  est->saturation = 1;
}

static inline uint32_t mib_clamp_pack(struct mib_rate_estimator* est, uint32_t value)
{
  return value < est->min_val_pack ? est->min_val_pack : value > est->max_val_pack ? est->max_val_pack : value; 
}

static inline uint32_t mib_clamp_time(struct mib_rate_estimator* est, uint32_t value)
{
  return value < est->min_val_time ? est->min_val_time : value > est->max_val_time ? est->max_val_time : value; 
}

static inline void calculate_optimum_occupancy(struct mib_rate_estimator* est)
{
  uint64_t pack_last_period = est->num_enqueued - est->num_enqueued_last;
  double occup = pack_last_period*((double)(est->discrete_time)/(double)(est->discrete_time_next_entity));
  est->opt_occupancy = mib_clamp(est,ceil(occup));
}

static inline void set_timestamp_enqueued_last(struct mib_rate_estimator* est, int64_t timestamp)
{
  est->slack_start_time = timestamp;
  est->num_enqueued_last = est->num_enqueued; 
} 

void mib_init_estimator(struct mib_rate_estimator* est)
{
	// val in microseconds
	est->min_val_time = 100; 
	est->max_val_time = 100000;

	est->min_val_pack = 1; 
	est->max_val_pack = 1024; 

	est->saturation = 0;
	
	est->slack_start_time =  mib_get_time_us();

	est->opt_occupancy = 1024;

	est->discrete_time = SDAP_SCHED_TICK ;
	est->discrete_time_next_entity = 100 * SDAP_SCHED_TICK;

	est->num_enqueued_last = 0;
	est->num_enqueued = 0; 
}

void mib_rate_enqueu(struct mib_rate_estimator* est, uint8_t numPackets)
{
  est->num_enqueued += numPackets; 
}

static inline void uint32_t calculate_max_num_pack(struct mib_rate_estimator* est, uint32_t discrete_periods)
{
  return  ((double)discrete_periods/0.8)*((double)est->discrete_time_next_entity/(double)est->discrete_time)*(double)est->opt_occupancy + est->num_enqueued_last;
}

void mib_rate_dequeue(struct mib_rate_estimator* est, uint8_t numPackets, int64_t timestamp)
{
  if(numPackets == 0){
    saturation_detected(est);  
    return;
  }
  
	const uint32_t discrete_periods = 3;

  if(est->saturation == 1){
    est->saturation = 0;
    est->discrete_time_next_entity = mib_clamp_time(timestamp - est->slack_start_time);
    calculate_optimum_occupancy(est);
    set_timestamp_enqueued_last(est, timestamp);
  } else if (timestamp > est->slack_start_time + discrete_periods*est->discrete_time_next_entity){
    // no buffer saturation detected. Increase the Opt. num packets 
    uint64_t max_limit = calculate_max_num_pack(est, discrete_periods); 
    if(max_limit <= est->num_queued){  // all possible packets passed){ 
			// double time and buffer size
      est->opt_occupancy = mib_clamp_pack(est,est->opt_occupancy*2);
      est->discrete_time_next_entity = mib_clamp_time(2*est->discrete_time_next_entity);
    }// else -- the upper layer did not provide this layer with enough packets, no congestion
    set_timestamp_enqueued_last(est, timestamp);
  }
  est->num_dequeued += numPackets;
}

uint8_t mib_get_optim_occupancy(struct mib_rate_estimator* est)
{
  return est->opt_occupancy;
}

