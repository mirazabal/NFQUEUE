#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>

#include "mib_SDAP_sched.h"
#include "mib_qfi_queues.h"
#include "mib_drb_queues.h"
#include "mib_time.h"

static const int forwardPacket = 1;
static int endThread = 1;
static const int maxNumberPacketsDRB = 20; 

void close_SDAP_thread()
{
  endThread = 0;
}
  
void* thread_SDAP_sched(void *threadData)
{
  struct SDAP_thread_data* data = (struct SDAP_thread_data*)threadData;

  const uint8_t QUEUE_QFI = 0;
  const uint8_t DRB_QUEUE_IDX = 0;
  while(endThread){
    usleep(1000);
    if(getDRBBufferStatus(data->drbQ, DRB_QUEUE_IDX) >= maxNumberPacketsDRB)
      continue;

    uint32_t queueSize = getQFIBufferStatus(data->qfiQ, QUEUE_QFI);
    printf("QFI queue size = %d at timestamp = %ld \n", queueSize , mib_get_time_us() ); 
    
    uint32_t counter = 0;
    while( (counter < 10) && queueSize != 0){
      uint32_t* idP = getQFIPacket(data->qfiQ,QUEUE_QFI);	
      if(idP == NULL)
        break;
      addPacketToDRB(data->drbQ, DRB_QUEUE_IDX, idP);
      //printf("Packet deque %lu \n",*idP);
   //   data->send_verdict_cb(data->NFQUEUE_NUM, *(uint32_t*)idP,forwardPacket);
    //  free(idP);	
      ++counter;
      --queueSize;
    }
  }
  return NULL;
}

