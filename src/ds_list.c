/**
*  Copyright (c) 2020 Surgams
*
*  This library is free software; you can redistribute it and/or modify it
*  under the terms of the BSD license. See LICENSE for details.
*
**/

#include "ds_list.h"

uint8_t ds_list_init (DS_List_Ptr *list, Destroy destroy) {
    /* Allocate list */
    if ((*list = (DS_List_Ptr) malloc(sizeof(DS_List))) == NULL)
        return ds_error_cannot_allocate_memory;

    /* Initialise the list */
    (*list)->size = 0;
    (*list)->head = NULL;
    (*list)->tail = NULL;
    (*list)->destroy = destroy;
    return ds_ok;
}

uint8_t ds_list_rem_next (DS_List_Ptr list, DS_Node_Ptr node, void **data) {
    DS_Node_Ptr old_node;
    
    if(!data)
        return ds_error_data_ptr_null;

    /* Do not allow removal from an empty list */
    if (ds_list_size(list) == 0)
        return ds_warning_empty;

    /* Remove the node from the list */
    if (node == NULL) {
        /* Handle removal from the head of the list */
        *data = list->head->data;
        old_node = list->head;
        list->head = list->head->next;

        if(ds_list_size(list) == 1) 
            list->tail = NULL;
    } else {
        /* Handle removal from somewhere other than the head */

        if (node->next == NULL)
            return ds_warning_last_element;

        *data = node->next->data;
        old_node = node->next;
        node->next = node->next->next;

        if (node->next == NULL)
            list->tail = node;
    }

    free(old_node);
    old_node = NULL;

    list->size--;
    return ds_ok;
}


uint8_t ds_list_purge (DS_List_Ptr list) {
    void *data;
    if (list->destroy != NULL) {
        uint8_t return_value = 0;
        /* Remove each node */
        while (ds_list_size(list) > 0) {
            if ((return_value = ds_list_rem_next (list, NULL, (void **)&data)) == ds_ok) {
                list->destroy(data);
            } else {
                return return_value;
            }
        }
        return ds_ok;
    }
    return ds_error_callback_pointer_null;
}

uint8_t ds_list_free (DS_List_Ptr *list) {
    uint8_t clean_return_value = 0;
    if ((clean_return_value = ds_list_purge(*list)) != ds_ok) 
        return clean_return_value;

    /* No operations are allowed now, but clear the structure as precaution */
    memset(*list, 0, sizeof(DS_List));

    free(*list);
    *list = NULL;
    return ds_ok;
}

uint8_t ds_list_add_next (DS_List_Ptr list, DS_Node_Ptr node, const void *data) {

    DS_Node *new_node;

    /* Allocate storage for the node */
    if ((new_node = (DS_Node_Ptr) malloc(sizeof(DS_Node))) == NULL)
        return ds_error_cannot_allocate_memory;

    /* Insert the node into the list */
    new_node->data = (void *)data;

    if (node == NULL) {
        /* Handle insertion at the head of the list */

        if(ds_list_size(list) == 0)
            list->tail = new_node;

        new_node->next = list->head;
        list->head = new_node;
    } else {
        /* Handle insertion somewhere other than at the head */

        if (node->next == NULL)
            list->tail = new_node;
        new_node->next = node->next;
        node->next = new_node;
    }

    list->size++;
    return ds_ok;
}

uint8_t ds_list_add_head (DS_List_Ptr list, const void *data) {
    return ds_list_add_next(list, NULL, data);
}

uint8_t ds_list_add_tail (DS_List_Ptr list, const void *data) {
    return ds_list_add_next(list, ds_list_tail(list), data);
}



