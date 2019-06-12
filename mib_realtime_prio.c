#include "mib_realtime_prio.h"

#include <stdio.h>
#include <sched.h>
#include <pthread.h>

void mib_set_realtime_priority(uint32_t priority)
{
  pthread_t this_thread = pthread_self();
  struct sched_param params;
  params.sched_priority = priority;
  int ret = pthread_setschedparam(this_thread, SCHED_RR, &params);
  if(ret != 0){
    printf("Error while setting the priority of this thread \n");
  }
}




