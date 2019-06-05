#include "mib_dyn_rlc.h"
#include "mib_time.h"

#include <math.h>
#include <stdio.h>

#define mib_max(a,b) \
({ __typeof__ (a) _a = (a); \
   __typeof__ (b) _b = (b); \
    _a > _b ? _a : _b; })


void mib_dyn_init(struct mib_dyn_rlc* dyn)
{
  dyn->interval_time_us = 200000; // time in us
  dyn->target_time_us = 7000;
  dyn->min_size = 2;
  dyn->max_size = 20;
  dyn->factor = 0.1;
  dyn->size = 10;
  dyn->sum_sojourn_times = 0; // sojourn_time;
  dyn->num_sum_sojourn_times = 0;
 dyn->last_time = mib_get_time_us();
}

void mib_dyn_release(struct mib_dyn_rlc* dyn)
{

}


uint32_t mib_dyn_size(struct mib_dyn_rlc* dyn)
{
  printf("Dyn size = %u \n", dyn->size);
  return dyn->size;
}

void mib_dyn_sojourn_time(struct mib_dyn_rlc* dyn, int64_t sojourn_time)
{
  int64_t now =  mib_get_time_us() ;

  dyn->sum_sojourn_times += sojourn_time;
  dyn->num_sum_sojourn_times += 1;

  if( dyn->last_time + dyn->interval_time_us > now)
    return;



  double sum_sojourn = dyn->sum_sojourn_times;
  double  target = dyn->num_sum_sojourn_times;
  double av_sojourn = sum_sojourn/target;
  printf("sum_sojourn =  %lf and target_time = %lf  ", sum_sojourn, target);
  printf("av_sojourn_time = %lf and target_time = %u  ",  av_sojourn, dyn->target_time_us);
  if(av_sojourn > dyn->target_time_us){
    uint32_t fact_val = dyn->size > dyn->min_size ?  dyn->size - dyn->min_size : 0;
    dyn->size = mib_max( dyn->size - mib_max(1.0, dyn->factor*fact_val  ) , dyn->min_size  );

  }else{ 
    uint32_t fact_val = dyn->size < dyn->max_size ?  dyn->max_size - dyn->size : 0;
    dyn->size =  dyn->size +  mib_max(1.0, dyn->factor*fact_val);
  }

  printf("New dyn size assigned = %u \n", dyn->size);
  dyn->sum_sojourn_times = 0; // sojourn_time;
  dyn->num_sum_sojourn_times = 0;

  dyn->last_time = now;
}


