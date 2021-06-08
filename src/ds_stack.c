/**
 * Copyright (c) 2020 Surgams
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the BSD license. See LICENSE for details.
 **/

#include "ds_stack.h"

ushort ds_stack_push (DS_Stack_Ptr stack, const void *data) {
    /* Push the data onto the stack */
    return ds_list_add_head(stack, data);
}

ushort ds_stack_pop (DS_Stack_Ptr stack, void **data) {
    /* Pop the data off the stack */
    return ds_list_rem_next(stack, NULL, data);
}
