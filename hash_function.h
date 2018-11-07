#ifndef MIB_HASH_FUNCTION
#define MIB_HASH_FUNCTION

#include <stddef.h>
#include <stdint.h>

uint32_t jenkins_one_at_a_time_hash(char* key, size_t len);

#endif

