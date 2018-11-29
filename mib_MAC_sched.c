#include "mib_MAC_sched.h"
#include "mib_drb_queues.h"
#include "mib_time.h"

#include <sched.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>
#include <stdio.h>

static const int forwardPacket = 1;
static int endThread = 1;

void close_MAC_thread()
{
  endThread = 0;
}

void* thread_MAC_sched(void* threadData)
{
  struct MAC_thread_data* data = (struct MAC_thread_data*)threadData;   

  uint8_t QUEUE_DRB = 0;
  while(endThread){
    usleep(10000);
    sched_yield();

    // loop throug the list of active queues taking into account the 
    uint32_t queueSize = getDRBBufferStatus(data->drbQ, QUEUE_DRB);
    printf("DRB queue size = %d at timestamp = %ld \n", queueSize , mib_get_time_us() ); 
    uint32_t counter = 0;
    while((counter < 10) && queueSize != 0){

      uint32_t* idP = getDRBPacket(data->drbQ,QUEUE_DRB);	
      if(idP == NULL)
        break;

      data->send_verdict_cb(data->NFQUEUE_NUM, *(uint32_t*)idP,forwardPacket);
      free(idP);	
      ++counter;
      --queueSize;
    }
  }
  return NULL;
}

