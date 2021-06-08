/**
*  Copyright (c) 2020 Surgams
*
*  This library is free software; you can redistribute it and/or modify it
*  under the terms of the BSD license. See LICENSE for details.
*
**/

#ifndef ARRAY_STACK_H
#define ARRAY_STACK_H


#include <stdint.h>
#include "ds_array_shared.h"
#include "ds_common.h"

/* Array Stack definition */
typedef DS_Array_Struct DS_A_Stack;
typedef DS_Array_Struct * DS_A_Stack_Ptr;
typedef DS_Array_Struct_Node DS_A_Stack_Node;
typedef DS_Array_Struct_Node * DS_A_Stack_Node_Ptr;

/* Array Stack interface */
uint8_t ds_array_stack_init (DS_A_Stack_Ptr *stack, const uint32_t size, Destroy destroy);
uint8_t ds_array_stack_push (DS_A_Stack_Ptr stack, const void *data);
uint8_t ds_array_stack_pop (DS_A_Stack_Ptr stack, void **data);
uint8_t ds_array_stack_free (DS_A_Stack_Ptr *stack);
uint8_t ds_array_stack_purge (DS_A_Stack_Ptr stack);

#define ds_array_stack_capacity(stack)((stack)->size - (stack)->index)
#define ds_array_stack_isfull(stack)((stack)->index < (stack)->size ? 0 : 1)
#define ds_array_stack_size(stack)((stack)->index)
#define ds_array_stack_allocated(stack)((stack)->size)

#endif
