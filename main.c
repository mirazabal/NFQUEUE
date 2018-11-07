#include "nf-queue.h"
#include "scheduler.h"

#include <stdio.h>
#include <stddef.h>

int main(int argc, char *argv[])
{
	if (argc != 2) {
		printf("Usage: %s [queue_num]\n", argv[0]);
		return -1;
	}
	unsigned int queue_num = atoi(argv[1]);


	init_nfqueue(queue_num,&add_packet_sched);
	init_sched(&send_verdict_nfqueue);
	
	for(;;)
	{
	
	}
	return 0;
}

