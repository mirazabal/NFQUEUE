#ifndef MIB_MAPPER
#define MIB_MAPPER

#include <stddef.h>
#include <stdint.h>

struct mib_mapper
{
	uint32_t* inputs;
	size_t sizeInputs;
	size_t sizeOutputs;
};


void mib_init_mapper(struct mib_mapper* map,size_t inputs, size_t outputs);

void mib_set_output_for_input(struct mib_mapper* map, uint32_t inputIdx, uint32_t outputIdx);

uint32_t mib_get_ouput_for_input(struct mib_mapper* map, uint32_t inputIdx);

void mib_free_mapper(struct mib_mapper* map);

#endif

