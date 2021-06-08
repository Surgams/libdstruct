/**
*  Copyright (c) 2020 Surgams
*
*  This library is free software; you can redistribute it and/or modify it
*  under the terms of the BSD license. See LICENSE for details.
*
**/

#ifndef ARRAY_SHARED_H
#define ARRAY_SHARED_H

#include <stdint.h>

/* Structs definitions */
typedef int (*Destroy) (void *data);

/* Node Structure */
typedef struct DS_Array_Struct_Node_ {
    void *data;
} DS_Array_Struct_Node;

/* Array Struct Structure */
typedef struct DS_Array_Struct_ {
    Destroy destroy;
    uint32_t size;
    uint32_t index;
    int32_t back_index;
    DS_Array_Struct_Node buffer[];
} DS_Array_Struct;

/* Array Struct definition */
typedef DS_Array_Struct * DS_Array_Struct_Ptr;
typedef DS_Array_Struct_Node * DS_Array_Struct_Node_Ptr;


#endif

