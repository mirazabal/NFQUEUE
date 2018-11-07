#include "nf-queue.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
	if (argc != 2) {
		printf("Usage: %s [queue_num]\n", argv[0]);
		return -1;
	}
	unsigned int queue_num = atoi(argv[1]);


	init_nfqueue(queue_num);

	for(;;)
	{
	
	}
	return 0;
}

