#include <assert.h>
#include <stdlib.h>
#include "mapper.h"

void mib_init_mapper(struct mib_mapper* map,size_t inputs, size_t outputs)
{
	map->inputs = malloc(inputs*sizeof(uint32_t));
	map->sizeInputs = inputs;
	map->sizeOutputs = outputs;
}

void mib_set_output_for_input(struct mib_mapper* map, uint32_t inputIdx, uint32_t outputIdx)
{
	assert(map->sizeInputs > inputIdx);
	assert(map->sizeOutputs > outputIdx);
	map->inputs[inputIdx] = outputIdx;
}

uint32_t get_ouput_for_input(struct mib_mapper* map, uint32_t inputIdx)
{
	assert(map->sizeInputs > inputIdx);
	return map->inputs[inputIdx];
}

void mib_free_mapper(struct mib_mapper* map)
{
	free(map->inputs);
}





