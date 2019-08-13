#include "mib_cqi_pacer.h"
#include "mib_time.h"

#include <assert.h>
#include <stdio.h>



void mib_cqi_pacer_set(struct mib_cqi_pacer* p, int32_t channel_buffer, int32_t remaining)
{
  assert(channel_buffer > -1 && remaining > -1);
  printf("Into mib_cqi_pacer_set, channel_buffer = %d, remaining = %d \n",channel_buffer, remaining);
  pthread_mutex_lock(&p->lock);
  p->pack_to_send = channel_buffer > remaining ? channel_buffer - remaining : 1;
  pthread_mutex_unlock(&p->lock);
  printf("Packets to send per TTI =  %u\n", p->pack_to_send);
  printf("Changed at 11th case\n");
  p->packets_tx = 0;
  p->slack_last_time = mib_get_time_us();
}

uint32_t mib_cqi_pacer_get_total_tx(struct mib_cqi_pacer* p)
{
    return p->pack_to_send;
}


uint32_t mib_cqi_pacer_get_opt(struct mib_cqi_pacer* p)
{
//  if(p->remaining_to_send <= 0){
//    printf("Into mib_cqi_pacer_get_opt with remaining_to_send = %d \n", p->remaining_to_send);
//    return 0;
//  }

  int64_t now = mib_get_time_us();
  int64_t time_ =  now - p->slack_last_time;
  float percentage_time =  time_ / p->TTI + 0.33;

  printf("percentage_time =  %f,  time_ = %ld \n", percentage_time, time_);
  if(percentage_time > 6){
    if(p->pack_to_send > 5){
        pthread_mutex_lock(&p->lock);
        p->pack_to_send = 0.75 * (float)p->pack_to_send;
        pthread_mutex_unlock(&p->lock);
    }
  }

  uint32_t theoretical_sent =  (float)(p->pack_to_send) * percentage_time;

  uint32_t ret_val = theoretical_sent > p->packets_tx ? theoretical_sent - p->packets_tx : 0;


//i  if(now < p->slack_last_time + 0.5*p->TTI){
//    ret_val = mib_clamp(p, p->pack_to_send / 10);
//  } else if (now < p->slack_last_time + 0.75*p->TTI){
//    ret_val = mib_clamp(p, 2 * p->pack_to_send / 10);
//  } else {
//    ret_val = p->remaining_to_send;
//  }
  printf("Into mib_cqi_pacer_get_opt, ret_val = %u,  tx = %u theoretical = %u  \n", ret_val,  p->packets_tx, theoretical_sent);
  return ret_val;
}

void mib_cqi_pacer_init(struct mib_cqi_pacer* p)
{
  p->slack_last_time = mib_get_time_us();
  p->pack_to_send = 5;
  p->TTI = 10000.0;
  p->packets_tx = 0;
  pthread_mutex_init(&p->lock, NULL);
}

void mib_cqi_pacer_enqueue(struct mib_cqi_pacer* p, uint32_t number)
{
  p->packets_tx += number;
  printf("Into mib_cqi_pacer_enqueue number = %u,  packet already sent = %d \n", number, p->packets_tx);
}


