#define _GNU_SOURCE
#include "nf-queue.h"
#include "5G_qos_model.h"

#include <sched.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>

// worse results observed from setting the affinity. Don't do it!!!
/*static void set_affinity()
{
	cpu_set_t mask;
	CPU_ZERO(&mask);
	const int core_id = 0;
	CPU_SET(core_id, &mask);
	sched_setaffinity( 0, sizeof(mask), &mask );
}
*/

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

