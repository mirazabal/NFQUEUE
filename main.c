#include "nf-queue.h"
#include "5G_qos_model.h"
#include <unistd.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  if (argc != 2) {
    printf("Usage: %s [queue_num]\n", argv[0]);
    return -1;
  }
  unsigned int queue_num = atoi(argv[1]);


  init_nfqueue(queue_num,&add_packet_5G_qos_model);
  init_5G_qos_model(&send_verdict_nfqueue);

  for(;;)
  {
    usleep(1000000);
  }
  return 0;
}

