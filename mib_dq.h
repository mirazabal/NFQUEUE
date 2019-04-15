#ifndef MIB_DYNAMIC_QUEUE
#define MIB_DYNAMIC_QUEUE

#include <stdatomic.h>
#include <stdint.h>

struct mib_dq
{
  uint64_t limit;
  _Atomic uint64_t num_queued;
  uint64_t num_dequeued;
  uint64_t lowest_slack;
  uint64_t max_limit;
  uint64_t min_limit;

  int64_t slack_start_time;
  int64_t slack_hold_time;
};

void mib_dq_init(struct mib_dq* dq, uint64_t hold_time_us);

void mib_dq_release(struct mib_dq*);

void mib_dq_reset(struct mib_dq* dq);

uint64_t mib_dq_avail(const struct mib_dq *dq);

void mib_dq_enqueued(struct mib_dq* dq, uint64_t count);

void mib_dq_dequeued(struct mib_dq* dq, uint64_t count);

uint32_t mib_dq_limit(const struct mib_dq *dq);

#endif //  MIB_DYNAMIC_QUEUE

