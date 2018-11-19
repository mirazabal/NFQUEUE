#ifndef MIB_RLC_BUFFER
#define MIB_RLC_BUFFER

struct RLCBuffer
{
  uint32_t buffer[1024];
  uint32_t bufferPos;
};

void initRLCBuffer(struct RLCBuffer* rlcB);
uint32_t getRLCBufferStatus(struct RLCBuffer* rlcB);
void addPacketToRLCBuffer(struct RLCBuffer* rlcB, uint32_t* id);
uint32_t* getRLCPacket(struct RLCBuffer* rlcB);

#endif

