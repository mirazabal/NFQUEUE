#include "mib_pacing.h"


static inline void saturation_detected(struct mib_rate_estimator* est, int64_t timestamp)
{
  est->saturation = 1;
}

static inline uint32_t mib_clamp_pack(struct mib_rate_estimator* est, uint32_t value)
{
  return value < est->min_val_pack ? est->min_val_pack : value > est->max_value_pack ? est->max_value_pack : value; 
}

static inline uint32_t mib_clamp_time(struct mib_rate_estimator* est, uint32_t value)
{
  return value < est->min_val_time ? est->min_val_time : value > est->max_value_time ? est->max_value_time : value; 
}

static inline void calculate_optimum_occupancy(struct mib_rate_estimator* est)
{
  uint64_t pack_last_period = est->num_queued - est->num_queued_last;
  double occup = pack_last_period*((double)(est->discrete_time)/(double)(est->discrete_time_next_entity));
  est->opt_occupancy = mib_clamp(est,ceil(occup));
}

static inline void set_timestamp_enqueued_last(struct mib_rate_estimator* est, int64_t timestamp)
{
  est->slack_start_time = timestamp;
  est->num_enqueued_last = est->num_queued; 
} 

void mib_rate_enqueu(struct mib_rate_estimator* est, uint8_t numPackets, int64_t timestamp)
{
  est->num_queued +=  numPackets; 
}

static inline void uint32_t calculate_max_num_pack(struct mib_rate_estimator* est)
{
  return 2.5*(est->discrete_time_next_entity/est->discrete_time)*est->opt_occupancy + est->num_enqueued_last;
}


void mib_rate_deque(struct mib_rate_estimator* est, int64_t timestamp, uint8_t numPackets)
{
  if(count == 0){
    saturation_detected(est);  
    return;
  }
  
  if(est->saturation == 1){
    est->saturation = 0;
    est->discrete_time_next_entity = mib_clamp_time(timestamp - est->slack_start_time);
    calculate_optimum_occupancy(est);
    set_timestamp_enqueued_last(est, timestamp);
  } else if  (timestamp > est->slack_start_time + 3*est->discrete_time_next_entity){
    // no buffer saturation detected. Increase the Opt. num packets 
    uint64_t max_limit = calculate_max_num_pack(est); 
    if(max_limit <= est->num_queued){  // all possible packets passed){ 
      est->opt_occupancy = mib_clamp_pack(est,est->opt_occupancy*2);
      est->discrete_time_next_entity = mib_clamp_time(2*est->ediscrete_time_next_entity);
    }// else -- the upper layer did not provide this layer with enough packets, no congestion
    set_timestamp_enqueued_last(est, timestamp);
  }
  est->num_dequeued += numPackets;
}

uint8_t mib_get_optim_occupancy(struct mib_rate_estimator* est)
{
  return est->opt_occupancy;
}

