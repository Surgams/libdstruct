/**
 * Copyright (c) 2020 bluecub
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the BSD license. See LICENSE for details.
 **/

#ifndef DS_QUEUE_H
#define DS_QUEUE_H


#include "ds_list.h"

/* Implement queues as linked lists */
typedef DS_List DS_Queue;
typedef DS_List_Ptr DS_Queue_Ptr;

/* PUblic Interface */

#define ds_queue_init ds_list_init
#define ds_queue_purge ds_list_purge
#define ds_queue_free ds_list_free

ushort ds_queue_enqueue (DS_Queue_Ptr queue, const void *data);
ushort ds_queue_dequeue (DS_Queue_Ptr queue, void **data);

#define ds_queue_peek(queue)((queue)->head == NULL ? NULL : (queue)->head->data)
#define ds_queue_size ds_list_size

#endif

