/**
 * Copyright (c) 2020 bluecub
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the BSD license. See LICENSE for details.
 **/

#ifndef LIST_H
#define LIST_H

#include "ds_common.h"

/* Linked list datastructure defintion */
typedef int (*Destroy) (void *data);
typedef void (*Display)(void *);
typedef int (*Compare)(void *key1, void *key2);

/*Elements Structure */
typedef struct DS_Node_ {
    void *data;
    struct DS_Node_ *next;
} DS_Node;


/* Linked list Structure */
typedef struct DS_List_ {
    int size;
    /* Callback methods */
    Compare compare;
    Destroy destroy;
    Display display;
    /* head and tail */
    DS_Node *head;
    DS_Node *tail;
} DS_List;

typedef DS_List * DS_List_Ptr;
typedef DS_Node * DS_Node_Ptr;

/* Public Interface */

ushort ds_list_init (DS_List_Ptr *list, Destroy destroy);
ushort ds_list_purge (DS_List_Ptr list);
ushort ds_list_free (DS_List_Ptr *list);
ushort ds_list_add_head (DS_List_Ptr list, const void *data);
ushort ds_list_add_tail (DS_List_Ptr list, const void *data);
ushort ds_list_add_next (DS_List_Ptr list, DS_Node_Ptr node, const void *data);

ushort ds_list_rem_next (DS_List_Ptr list, DS_Node_Ptr node, void **data);

#define ds_list_size(list)((list)->size)
#define ds_list_head(list)((list)->head)
#define ds_list_tail(list)((list)->tail)
#define ds_list_is_head(list, node)((node) == (list)->head ? 1 : 0)
#define ds_list_is_tail(node)((node)->next == NULL ? 1 : 0)
#define ds_list_data(node)((node)->data)
#define ds_list_next(node)((node)->next)

#endif







