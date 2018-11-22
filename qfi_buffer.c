#include "qfi_buffer.h"


void initQFIBuffer(struct QFIBuffer* qfiB)
{
	qfiB->it_first=0;
	qfiB->it_last=0;
}

uint32_t getQFIBufferStatus(struct QFIBuffer* qfiB)
{
	if(qfiB->it_first <= qfiB->it_last)
	 	return qfiB->it_last - qfiB->it_first;

	return (QFI_BUFFER_SIZE - qfiB->it_first) + qfiB->it_last; 
}

void addPacketToQFIBuffer(struct QFIBuffer* qfiB, uint32_t* id)
{
	qfiB->buffer[qfiB->it_last] = id;
	qfiB->it_last++;
	if(qfiB->it_last == QFI_BUFFER_SIZE)
		qfiB->it_last = 0;
}

uint32_t* getQFIPacket(struct QFIBuffer* qfiB)
{
	uint32_t* ret = qfiB->buffer[qfiB->it_first];
	qfiB->it_first++;
 	if( qfiB->it_first == QFI_BUFFER_SIZE)
		qfiB->it_first = 0;

	return ret;
}


