#include "ds_array_queue.h"


ushort inline static has_capacity(uint32_t size, uint32_t findex, int bindex) {
    /* Check if the queue is empty */
    if (bindex == -1 && findex == 0)
        return ds_warning_empty;

    /* Full ..... */
    if ((bindex > 0 && bindex == (int32_t)findex) || 
            (bindex == 0 && findex == size))
        return ds_warning_buffer_is_full;

    return ds_ok;
}

ushort ds_array_queue_init (DS_A_Queue_Ptr *queue, const uint32_t size, Destroy destroy) {
    /* Allocate list */
    if ((*queue = (DS_A_Queue_Ptr) malloc(sizeof(DS_A_Queue) * size * 
                    sizeof(DS_A_Queue_Node))) == NULL)
        return ds_error_cannot_allocate_memory;

    /* Initialise the queue */
    (*queue)->size = size;
    (*queue)->index = 0;
    (*queue)->back_index = -1;
    (*queue)->destroy = destroy;
    return ds_ok;
}

ushort ds_array_queue_enqueue (DS_A_Queue_Ptr queue, const void *data) {
    DS_A_Queue_Node_Ptr node;
    ushort capacity = has_capacity(queue->size, queue->index, queue->back_index);

    if (capacity == ds_warning_buffer_is_full)
        return capacity;

    if ((node = (DS_A_Queue_Node_Ptr) malloc(sizeof(DS_A_Queue_Node))) == NULL)
        return ds_error_cannot_allocate_memory; 
    node->data = (void *)data;
    queue->buffer[queue->index] = *node;

    if (queue->index == queue->size)
        queue->index = 0;

    queue->index++;

    if (capacity == ds_warning_empty)
        queue->back_index++;

    return ds_ok;
}

ushort ds_array_queue_dequeue (DS_A_Queue_Ptr queue, void **data) {
    if(!data)
        return ds_error_data_ptr_null;

    /* Do not allow removal from an empty queue */
    if (has_capacity(queue->size, queue->index, queue->back_index) == ds_warning_empty)
        return ds_warning_empty;

    int b_index = queue->back_index;
    *data = queue->buffer[b_index].data;
    /* Free and clean the node after dequeue */
    free(&(queue->buffer[b_index]));
    queue->back_index++;
    if (queue->back_index > 0 && queue->back_index == (int32_t)queue->index) {
        queue->back_index = -1;
        queue->index = 0;
    }
    return ds_ok;
}

ushort ds_array_queue_free (DS_A_Queue_Ptr *queue) {
    uint32_t size = (*queue)->size;

    if ((*queue)->destroy == NULL) 
        return ds_error_callback_pointer_null;

    for (uint32_t i = 0; i < size; i++) {
        (*queue)->destroy(&((*queue)->buffer[i].data));
        free(&((*queue)->buffer[i]));
    }    
    (*queue)->back_index = -1;
    (*queue)->index = 0;
    (*queue)->size = 0;
    free(*queue);
    return ds_ok;
} 

ushort ds_array_queue_purge (DS_A_Queue_Ptr queue) {
    uint32_t size = queue->size;

    if (queue->destroy == NULL) 
        return ds_error_callback_pointer_null;

    for (uint32_t i = 0; i < size; i++) {
        queue->destroy(queue->buffer[i].data);
    }    
    queue->back_index = -1;
    queue->index = 0;
    return ds_ok;
}

ushort ds_array_queue_used(DS_A_Queue_Ptr queue) {
    uint32_t index, size;
    int32_t back_index;

    index = queue->index;
    size = queue->size;
    back_index = queue->back_index;

    if (back_index == -1 && index == 0)
        return 0;
    else if ((int32_t)index > back_index)
        return index - back_index;
    else
        return (size - back_index) + index;
}


ushort ds_array_queue_capacity(DS_A_Queue_Ptr queue) {
    uint32_t used = ds_array_queue_used(queue);
    return queue->size - used;
}

