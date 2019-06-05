#ifndef MIB_DYN_RLC
#define MIB_DYN_RLC

#include <stdint.h>


struct mib_dyn_rlc
{
  uint64_t last_time;
  uint32_t interval_time_us;
  uint32_t target_time_us;

  uint32_t min_size;
  uint32_t max_size;
  float factor;
  uint32_t size;

  int64_t sum_sojourn_times;
  int64_t num_sum_sojourn_times;

};


void mib_dyn_init(struct mib_dyn_rlc* dyn);

void mib_dyn_release(struct mib_dyn_rlc* dyn);

void mib_dyn_sojourn_time(struct mib_dyn_rlc* dyn, int64_t time);

uint32_t mib_dyn_size(struct mib_dyn_rlc* dyn);


#endif

