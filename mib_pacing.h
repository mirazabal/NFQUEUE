#ifndef MIB_PACING
#define MIB_PACING


struct mib_pacing
{
  uint32_t discrete_time;

};





struct mib_rate_estimator
{

  uint64_t num_queued;
  uint64_t num_dequeued;
  uint64_t lowest_slack;
  uint64_t max_limit;
  uint64_t min_limit;

  int64_t slack_start_time;
  int64_t slack_hold_time;


  uint32_t const discrete_time;
  uint32_t discrete_time_next_entity;

  uint32_t number_packets_added;
  uint32_t number_packets_added_last;

  int32_t start_timestamp_us;
  int32_t start_timestamp_us_last;

  uint32_t end_timestamp_us;
  uint8_t saturation_detected;
};


void mib_packets_added(struct mib_rate_estimator* est, uint8_t numPackets, int64_t timestamp);




void mib_rate_dequeue(struct mib_rate_estimator* est, uint8_t numPackets);

void mib_rate_enqueu(struct mib_rate_estimator* est, uint8_t numPackets, int64_t timestamp);


#endif
