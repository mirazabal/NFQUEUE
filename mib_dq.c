#include "mib_dq.h"
#include "mib_time.h"

#include <assert.h>
#include <limits.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define POSDIFF(A, B) ((int)((A) - (B)) > 0 ? (A) - (B) : 0)

static inline uint64_t mib_clamp(uint64_t val, uint64_t min, uint64_t max)
{
  return val < min ? min : val > max ? max : val; 
}

void mib_dq_init(struct mib_dq* dq, uint64_t hold_time_us)
{
  dq->min_limit = 1;
  dq->max_limit = 512;
  dq->limit = 25;
  dq->slack_hold_time = hold_time_us;
  mib_dq_reset(dq);
}

void mib_dq_release(struct mib_dq* dq)
{
  free(dq);
}

uint32_t mib_dq_limit(const struct mib_dq *dq)
{
	return dq->limit;
}

void mib_dq_reset(struct mib_dq* dq)
{
  dq->num_queued = 0;
  dq->num_dequeued = 0;
  dq->lowest_slack = UINT_MAX;
  dq->slack_start_time = mib_get_time_us();
}

uint64_t mib_dq_avail(const struct mib_dq *dq)
{
  //	assert(dq->num_dequeued + dq->limit >= dq->num_queued );
  // num_dequeud + limit can be lower than num_queue just when a new value is assigned to limit 	
  uint64_t maxNumPacAllowed = dq->num_dequeued + dq->limit;
  uint64_t num_queued =  dq->num_queued;
	uint64_t avail =  POSDIFF( maxNumPacAllowed, num_queued);
  printf("available packets = %lu, dq->num_dequeued = %lu dq->limit =  %lu and num_queued = %lu \n", avail, dq->num_dequeued, dq->limit, num_queued);
  return avail;
}

void mib_dq_enqueued(struct mib_dq* dq, uint64_t count)
{
  atomic_fetch_add_explicit(&dq->num_queued, count, memory_order_relaxed); 	
//  printf("mib_dq_enqueued =  %lu \n", dq->num_queued);
}

void mib_dq_dequeued(struct mib_dq* dq, uint64_t count)
{

  //	printf("Before function dequeued after count = %lu/n", count);
  if(count == 0){
    dq->slack_start_time =  mib_get_time_us();
    dq->lowest_slack = UINT_MAX;
    return;	
  }
  //	printf("Into function dequeued after count = %lu/n", count);
  assert(count <= dq->num_queued - dq->num_dequeued);
  // num_dequeud + limit can be lower than num_queue just when a new value is assigned to limit 	
  //assert(dq->num_queued - dq->num_dequeued <= dq->limit);

  uint64_t num_queued = dq->num_queued;
  uint8_t limit_reached = dq->limit == num_queued - dq->num_dequeued ? 1:0;
  uint64_t num_dequeued = dq->num_dequeued + count;
  uint64_t remaining_data = num_queued - num_dequeued; 
  uint64_t limit = dq->limit;

  printf("limit reached = %d  and remaining data =  %lu with count = %lu  \n", limit_reached ,remaining_data, count);
  if(limit_reached && !remaining_data){
    //const float sqrt_2 = 1.41;
		// limit =  sqrt_2 * limit;	
    const float increase_factor = 1.2;
		limit = increase_factor * limit;
    dq->slack_start_time = mib_get_time_us();	
    dq->lowest_slack = UINT_MAX;
  } else if(remaining_data){
    //uint64_t slack = limit - count; 	
    if (remaining_data < dq->lowest_slack){
      dq->lowest_slack = remaining_data; //slack;
      printf("remaining data  = %lu \n", remaining_data);
    }
    int64_t time_us = mib_get_time_us();
    if( time_us > dq->slack_start_time + dq->slack_hold_time){
      limit = POSDIFF(limit, dq->lowest_slack);
      dq->slack_start_time = time_us;
      dq->lowest_slack = UINT_MAX;
    }
  } else if(!limit_reached && !remaining_data ){
    dq->slack_start_time = mib_get_time_us();	
    dq->lowest_slack = UINT_MAX;
  }

  if(limit != dq->limit){
    dq->limit = mib_clamp(limit, dq->min_limit, dq->max_limit);
  }

  printf("dq limit value = %lu with count = %lu at %lu  \n", dq->limit, count, mib_get_time_us());
  if(limit != dq->limit)
    limit = dq->limit;

  dq->num_dequeued = num_dequeued;
}

