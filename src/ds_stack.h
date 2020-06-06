#ifndef DS_STACK_H
#define DS_STACK_H


#include "ds_list.h"

/* Implement stacks as linked lists */
typedef DS_List DS_Stack;
typedef DS_List_Ptr DS_Stack_Ptr;

/* Public interface */

#define ds_stack_init ds_list_init
#define ds_stack_purge ds_list_purge
#define ds_stack_free ds_list_free

ushort ds_stack_push(DS_Stack_Ptr, const void *);
ushort ds_stack_pop(DS_Stack_Ptr, void **);

#define ds_stack_peek(stack)((stack)->head == NULL ? NULL : (stack)->head->data)
#define ds_stack_size ds_list_size

#endif
