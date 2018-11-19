#ifndef MIB_RLC_BUFFER
#define MIB_RLC_BUFFER

#define RLC_BUFFER_SIZE 1024
#include <stdint.h>

struct RLCBuffer
{
  uint32_t* buffer[RLC_BUFFER_SIZE];
  uint32_t it_first;
  uint32_t it_last;
};

void initRLCBuffer(struct RLCBuffer* rlcB);
uint32_t getRLCBufferStatus(struct RLCBuffer* rlcB);
void addPacketToRLCBuffer(struct RLCBuffer* rlcB, uint32_t* id);
uint32_t* getRLCPacket(struct RLCBuffer* rlcB);

#endif

