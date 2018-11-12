#ifndef MIB_LIST_H
#define  MIB_LIST_H

#include <stdlib.h>


typedef struct mib_list_t {
  struct mem_block_t *head;
  struct mem_block_t *tail;
  size_t nb_elements;
} mib_list_t;


void mib_list_init (mib_list_t*);
void mib_list_free (mib_list_t* listP);
mem_block_t* mib_list_get_head (mib_list_t*);
mem_block_t* mib_list_get_head_or_drop(mib_list_t*, rlc_buffer_occupancy_t*);
void mib_list_remove_head (mib_list_t*);
void mib_list_add_tail(mem_block_t* , mib_list_t* );

#endif // MIB_LIST_H

