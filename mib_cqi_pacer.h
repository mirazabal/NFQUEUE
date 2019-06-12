#ifndef MIB_CQI_PACER
#define MIB_CQI_PACER
#include <stdint.h>
#include <stdatomic.h>

struct mib_cqi_pacer
{
  int64_t slack_last_time;
  uint32_t pack_to_send;
  _Atomic int32_t remaining_to_send;
  float TTI;
  uint32_t min_val_pack;
  uint32_t max_val_pack;
};


void mib_cqi_pacer_set(struct mib_cqi_pacer* p, int32_t channel_buffer, int32_t remaining);
uint32_t mib_cqi_pacer_get_opt(struct mib_cqi_pacer* p);
void mib_cqi_pacer_init(struct mib_cqi_pacer* p);
void mib_cqi_pacer_enqueue(struct mib_cqi_pacer* p, uint32_t number);

#endif
