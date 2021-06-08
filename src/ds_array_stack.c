/**
 * Copyright (c) 2020 Surgams
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the BSD license. See LICENSE for details.
 **/

#include "ds_array_stack.h"


ushort ds_array_stack_init (DS_A_Stack_Ptr *stack, const uint32_t size, Destroy destroy) {
    /* Allocate list */
    if ((*stack = (DS_A_Stack_Ptr) malloc(sizeof(DS_A_Stack) * size * sizeof(DS_A_Stack_Node))) == NULL)
        return ds_error_cannot_allocate_memory;

    /* Initialise the stack */
    (*stack)->size = size;
    (*stack)->index = 0;
    (*stack)->destroy = destroy;
    return ds_ok;

}
ushort ds_array_stack_push (DS_A_Stack_Ptr stack, const void *data) {
    DS_A_Stack_Node_Ptr node;
    if (ds_array_stack_isfull(stack))
        return ds_warning_buffer_is_full;

    if ((node = (DS_A_Stack_Node_Ptr) malloc(sizeof(DS_A_Stack_Node))) == NULL)
        return ds_error_cannot_allocate_memory; 
    node->data = (void *)data;
    stack->buffer[stack->index] = *node;
    stack->index++;
    return ds_ok;
}

ushort ds_array_stack_pop (DS_A_Stack_Ptr stack, void **data) {
    if(!data)
        return ds_error_data_ptr_null;

    /* Do not allow removal from an empty stack */
    if (stack->index == 0)
        return ds_warning_empty;

    int index = stack->index -1;
    *data = stack->buffer[index].data;
    /* Free and clean the node after poping */
    free(&(stack->buffer[index]));
    stack->index--;

    return ds_ok;
}

ushort ds_array_stack_free (DS_A_Stack_Ptr *stack) {
    ushort ret_val;
    if ((ret_val = ds_array_stack_purge (*stack)) != ds_ok)
        return ret_val;
    
    free(*stack);

    return ds_ok;
} 

ushort ds_array_stack_purge (DS_A_Stack_Ptr stack) {
    uint32_t local_index = stack->index;

    if (stack->destroy == NULL) 
        return ds_error_callback_pointer_null;
    
    for (uint32_t i = 0; i < local_index; i++) {
        stack->destroy(stack->buffer[i].data);
        stack->index--;
        free(&stack->buffer[i]);
    }    
    return ds_ok;
}
