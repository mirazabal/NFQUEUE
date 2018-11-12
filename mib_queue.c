#include "mib_queue.h"
#include "mib_queue_lfds_impl.h"
#include <stdint.h>
#include <stdio.h>

static int const queue_num = 0;

void mib_queue_init(void (*cb_)(uint32_t, uint32_t, uint32_t))
{
	//send_verdict_cb = cb_;
	mib_queue_lfds_init();
}

void mib_queue_enqueu(void* data)
{
	mib_queue_lfds_enqueu(data);
}

void* mib_queue_deque()
{
	void* ret = mib_queue_lfds_deque();
	if(ret == NULL) 	return ret;
	return ret; 
}

size_t mib_queue_size()
{
	return  mib_queue_lfds_size();
}

void mib_queue_free()
{
	mib_queue_lfds_free();
}

