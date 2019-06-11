#include "mib_cqi_pacer.h"
#include "mib_time.h"

#include <stdio.h>

static uint32_t mib_clamp(struct mib_cqi_pacer* p, float value)
{
  return value > p->max_val_pack ? p->max_val_pack : value < p->min_val_pack ? p->min_val_pack : value;
}

void mib_cqi_pacer_set(struct mib_cqi_pacer* p, uint32_t channel_buffer, uint32_t remaining)
{
  printf("Into mib_cqi_pacer_set, channel_buffer = %u, remaining = %u \n",channel_buffer, remaining);
  p->remaining_to_send = channel_buffer > remaining ? channel_buffer - remaining : 0;
  p->pack_to_send = p->remaining_to_send;
  p->slack_last_time = mib_get_time_us();
}

uint32_t mib_cqi_pacer_get_opt(struct mib_cqi_pacer* p)
{
  if(p->remaining_to_send == 0) return 0;

  int64_t now = mib_get_time_us();
  uint32_t ret_val = 0;
  if(now < p->slack_last_time + 0.5*p->TTI){
    ret_val = mib_clamp(p, p->pack_to_send / 10);
  } else if (now < p->slack_last_time + 0.75*p->TTI){
    ret_val = mib_clamp(p, 2 * p->pack_to_send / 10);
  } else {
    ret_val = p->remaining_to_send;
  }
  printf("Into mib_cqi_pacer_get_opt, ret_val = %u,  remaining = %u \n", ret_val, p->remaining_to_send);
  return ret_val;
}

void mib_cqi_pacer_init(struct mib_cqi_pacer* p)
{
  p->slack_last_time = mib_get_time_us();
  p->remaining_to_send = 0;
  p->pack_to_send = 0;
  //p->channel_buffer = 10;
  //p->pac_to_send = 0;
  //p->pac_remaining_to_send = 0;
  p->TTI = 10000;
  p->min_val_pack = 0;
  p->max_val_pack = 15;
}

void mib_cqi_pacer_enqueue(struct mib_cqi_pacer* p, uint32_t number)
{
  p->remaining_to_send -= number;
  printf("Into mib_cqi_pacer_enqueue number = %u,  remaining = %u \n", number, p->remaining_to_send);
}


