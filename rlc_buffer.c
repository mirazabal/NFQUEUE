#include "rlc_buffer.h"


void initRLCBuffer(struct RLCBuffer* rlcB)
{
	rlcB->it_first=0;
	rlcB->it_last=0;
}

uint32_t getRLCBufferStatus(struct RLCBuffer* rlcB)
{
	if(rlcB->it_first <= rlcB->it_last)
	 	return rlcB->it_last - rlcB->it_first;

	return (RLC_BUFFER_SIZE - rlcB->it_first) + rlcB->it_last; 
}

void addPacketToRLCBuffer(struct RLCBuffer* rlcB, uint32_t* id)
{
	rlcB->buffer[rlcB->it_last] = id;
	rlcB->it_last++;
	if(rlcB->it_last == RLC_BUFFER_SIZE)
		rlcB->it_last = 0;
}

uint32_t* getRLCPacket(struct RLCBuffer* rlcB)
{
	uint32_t* ret = rlcB->buffer[rlcB->it_first];
	rlcB->it_first++;
 	if( rlcB->it_first == RLC_BUFFER_SIZE)
		rlcB->it_first = 0;

	return ret;
}


