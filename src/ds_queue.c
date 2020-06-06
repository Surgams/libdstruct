#include "ds_queue.h"

/* queue_enqueue */
ushort ds_queue_enqueue (DS_Queue_Ptr queue, const void *data) {
    return ds_list_add_tail(queue, data);
}

/* queue_dequeue */
ushort ds_queue_dequeue (DS_Queue_Ptr queue, void **data) {
    return ds_list_rem_next(queue, NULL, data);
}



