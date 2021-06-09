/**
*  Copyright (c) 2020 Surgams
*
*  This library is free software; you can redistribute it and/or modify it
*  under the terms of the BSD license. See LICENSE for details.
*
**/

#ifndef DS_HASH_H
#define DS_HASH_H

#include <stdint.h>
#include "ds_list.h"

/* Define structure for the hash node*/
typedef struct DS_Hash_Node_ {
    char *key;
    void *data;
} DS_Hash_Node;

typedef DS_Hash_Node * DS_Hash_Node_Ptr;

/* Define a structure for chained hash table */
typedef unsigned (*Hash)(const char *key);

typedef struct DS_Hash_Table_ {
    int buckets;
    Compare compare;
    Destroy destroy;
    Hash hash;
    int size;
    DS_List_Ptr table;
} DS_Hash_Table;

typedef DS_Hash_Table * DS_Hash_Table_Ptr;

/* Public Interface */

uint8_t ds_hash_init (DS_Hash_Table_Ptr *htbl, int buckets, Compare compare, Destroy destroy);

uint8_t ds_hash_free (DS_Hash_Table_Ptr *htbl);
uint8_t ds_hash_put (DS_Hash_Table_Ptr htbl, const char *key, const void *data);
uint8_t ds_hash_remove (DS_Hash_Table_Ptr htbl, const char *key, void **data);
uint8_t ds_hash_get (const DS_Hash_Table_Ptr htbl, const char *key, void **data);
uint8_t ds_hash_iterate_keys (const DS_Hash_Table_Ptr htbl, char *keys[], int keys_size);

#define ds_hash_size(htbl) ((htbl)->size)

#endif
