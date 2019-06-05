#include "mib_read_chan.h" 
#include "mib_data_arr.h"

#include <stdio.h>
#include <stdlib.h>

struct data_arr read_channel_data(const char* fileName)
{
  struct data_arr arr;
  FILE* file = fopen(fileName, "r"); /* should check the result */
  char line[256];
  fgets(line, sizeof(line), file);
  arr.size = (uint32_t)strtol(line, (char **)NULL, 10);
  arr.data = malloc(sizeof(uint32_t )*arr.size);
  uint32_t i = 0;
  while (fgets(line, sizeof(line), file)) {
    arr.data[i] = (uint32_t)strtol(line, (char **)NULL, 10);
    ++i;
  }
  fclose(file);
  return arr;
}
