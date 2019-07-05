#include "5G_qos_model.h"
#include "mib_queue_codel.h"
#include "mib_queue.h"
#include "mib_time.h"
#include "mib_scenario.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

static const int queueNum = 0;
static const int dropPacket = 0;

static void (*send_verdict_cb)(uint32_t, uint32_t, uint32_t);
static struct stats_t* codel_stats;

static void init_codel_params(struct QueueCodel* queue)
{
  queue->first_above_time_ = 0; 
  queue->drop_next_ = 0;
  queue->count_ = 0;
  queue->lastcount_ = 0;
  queue->dropping_ = 0; // {false}
  queue->pTimer.pos = 0;
	queue->q = malloc(sizeof(struct LockFreeQueue));
  mib_queue_init(queue->q/*,verdict*/);

  queue->interval_ = 40000;
  queue->target_   =  8000;
}

static void init_mib_stats(struct stats_t* s)
{
  codel_stats = s;  
}

static int64_t control_law(struct QueueCodel* queue, int64_t t, uint32_t count)
{
  double temp =  (double)queue->interval_ / sqrt( (double)count );
  return t +  (int64_t)temp;
}

struct dodequeue_result
{
  void* m;
  uint32_t ok_to_drop;
};

static struct dodequeue_result dodequeue(struct QueueCodel* queue, int64_t now)
{
  void* p = mib_queue_deque(queue->q);
  if(p == NULL){
    struct dodequeue_result ret = { .m = p, .ok_to_drop = 0};
    return ret;
  }

  int64_t sojourn_time = now - queue->pTimer.usecs[queue->pTimer.pos-1];
  if (sojourn_time < queue->target_ ){ //|| bytes() <= maxpacket_) 
    // went below - stay below for at least INTERVAL
    queue->first_above_time_ = 0; // time_stamp(std::chrono::microseconds(0));
    struct dodequeue_result ret = { .m = p, .ok_to_drop = 0};
    return ret;
  }

  if (queue->first_above_time_ == 0){ // time_stamp(std::chrono::microseconds(0))) 
    // just went above from below. if still above at
    // first_above_time, will say it’s ok to drop.
    queue->first_above_time_ = now + queue->interval_; // INTERVAL;
  } else if((now >= queue->first_above_time_) ){
    //printf("Sojourn time = %lld \n", (long long)sojourn_time);
    struct dodequeue_result ret = { .m = p, .ok_to_drop = 1};
    return ret;
    //return {&p, true}; // ok_to_drop = true;
  }
  struct dodequeue_result ret = { .m = p, .ok_to_drop = 0};
  return ret;
}

void mib_queue_codel_init(struct QueueCodel* queue, void(*verdict)(uint32_t, uint32_t, uint32_t), struct stats_t* stats)
{
  init_codel_params(queue);
  init_mib_stats(stats);
  send_verdict_cb = verdict;
}

static void drop_packet(struct QueueCodel* queue)
{
  void* data = mib_queue_deque(queue->q);
  if(data == NULL) return;

  send_verdict_cb( queueNum, ((struct packet_t*)data)->idP, dropPacket);
  free(data);
  queue->packets_dropped++;
  printf(" dropping packet.. %lu at timestamp %lu \n", queue->packets_dropped , mib_get_time_us()  );
  queue->pTimer.pos = queue->pTimer.pos - 1;

#if QFI_QUEUES_CODEL || DRB_QUEUES_CODEL  
  mib_remove_packet_QFI_DRB(codel_stats);
#endif

}


void* mib_queue_codel_deque(struct QueueCodel* queue)
{
  int64_t now = mib_get_time_us();
  struct dodequeue_result r = dodequeue(queue,now);
  if(r.m == NULL){
    queue->first_above_time_ = 0; //time_stamp(std::chrono::microseconds(0));
    return NULL;
  }

  if (queue->dropping_ == 1) {
    if ( r.ok_to_drop == 0) { //false
      // sojourn time below TARGET - leave drop state
      queue->dropping_ = 0;// false;
      return r.m;
    }
    while (now >= queue->drop_next_ && (queue->dropping_ == 1)) {
      drop_packet(queue); // free_sdu(listP, rlc_boP);

      if(mib_queue_size(queue->q) == 0){
        queue->first_above_time_ = 0; //time_stamp(std::chrono::microseconds(0));
        return NULL;
      }
      r = dodequeue(queue,now);
      if(r.m == NULL){
        queue->first_above_time_ = 0; //time_stamp(std::chrono::microseconds(0));
        return NULL;
      }

      if (r.ok_to_drop == 0 || r.m == NULL) { //false
        // leave drop state
        queue->dropping_ = 0;// false;
      } else {
        ++queue->count_;
        // schedule the next drop.
        queue->drop_next_ = control_law(queue, queue->drop_next_, queue->count_);
      }
    }
  } else if (r.ok_to_drop == 1) {
    drop_packet(queue); // free_sdu(listP, rlc_boP);
    if(mib_queue_size(queue->q) == 0){
      queue->first_above_time_ = 0; //time_stamp(std::chrono::microseconds(0));
      return NULL;
    }
    r = dodequeue(queue,now);
    if(r.m == NULL){
      queue->first_above_time_ = 0; //time_stamp(std::chrono::microseconds(0));
      return NULL;
    }
    queue->dropping_ = 1;// true;
    uint32_t delta = queue->count_ - queue->lastcount_;
    queue->count_ = 1;
    if ((delta > 1) && (now - queue->drop_next_ < 16*queue->interval_))
      queue->count_ = delta;
    queue->drop_next_ = control_law(queue,now, queue->count_);
    queue->lastcount_ = queue->count_;
  }
  queue->pTimer.pos = queue->pTimer.pos - 1;
  return r.m;
}

void mib_queue_codel_enqueu(struct QueueCodel* queue, void* data)
{
  mib_queue_enqueu(queue->q,data);
  queue->pTimer.usecs[queue->pTimer.pos] = mib_get_time_us();
  queue->pTimer.pos = queue->pTimer.pos + 1;
}

size_t mib_queue_codel_size(struct QueueCodel* queue)
{
  return mib_queue_size(queue->q);
}

void mib_queue_codel_free(struct QueueCodel* queue)
{
  mib_queue_free(queue->q);
}

