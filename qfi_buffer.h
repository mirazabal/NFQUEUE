#ifndef MIB_QFI_BUFFER
#define MIB_QFI_BUFFER

#define QFI_BUFFER_SIZE 1024
#include <stdint.h>

struct QFIBuffer
{
  uint32_t* buffer[QFI_BUFFER_SIZE];
  uint32_t it_first;
  uint32_t it_last;
};

void initQFIBuffer(struct QFIBuffer* qfiB);
uint32_t getQFIBufferStatus(struct QFIBuffer* qfiB);
void addPacketToQFIBuffer(struct QFIBuffer* qfiB, uint32_t* id);
uint32_t* getQFIPacket(struct QFIBuffer* qfiB);

#endif

