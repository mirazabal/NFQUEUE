#include "mib_queue.h"
#include "mib_queue_lfds_impl.h"


void mib_queue_init()
{
	mib_queue_lfds_init();
}

void mib_queue_enqueu(void* data)
{
	mib_queue_lfds_enqueu(data);
}

void* mib_queue_deque()
{
	return mib_queue_lfds_deque();
}

size_t mib_queue_size()
{
	return  mib_queue_lfds_size();
}

void mib_queue_free()
{
	mib_queue_lfds_free();
}

