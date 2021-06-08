/**
*  Copyright (c) 2020 Surgams
*
*  This library is free software; you can redistribute it and/or modify it
*  under the terms of the BSD license. See LICENSE for details.
*
**/

#ifndef ARRAY_QUEUE_H
#define ARRAY_QUEUE_H

#include <stdint.h>
#include "ds_common.h"
#include "ds_array_shared.h"

/* Array Queue definition */
typedef DS_Array_Struct DS_A_Queue;
typedef DS_Array_Struct * DS_A_Queue_Ptr;
typedef DS_Array_Struct_Node DS_A_Queue_Node;
typedef DS_Array_Struct_Node * DS_A_Queue_Node_Ptr;

/* Array Queue interface */
uint8_t ds_array_queue_init (DS_A_Queue_Ptr *queue, const uint32_t size, Destroy destroy);
uint8_t ds_array_queue_enqueue (DS_A_Queue_Ptr queue, const void *data);
uint8_t ds_array_queue_dequeue (DS_A_Queue_Ptr queue, void **data);
uint8_t ds_array_queue_free (DS_A_Queue_Ptr *queue);
uint8_t ds_array_queue_purge (DS_A_Queue_Ptr queue);
uint8_t ds_array_queue_capacity (DS_A_Queue_Ptr queue);
uint8_t ds_array_queue_used (DS_A_Queue_Ptr queue);

#endif

