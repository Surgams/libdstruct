/**
 * Copyright (c) 2020 Surgams
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the BSD license. See LICENSE for details.
 **/

#include "ds_queue.h"

/* queue_enqueue */
ushort ds_queue_enqueue (DS_Queue_Ptr queue, const void *data) {
    return ds_list_add_tail(queue, data);
}

/* queue_dequeue */
ushort ds_queue_dequeue (DS_Queue_Ptr queue, void **data) {
    return ds_list_rem_next(queue, NULL, data);
}



