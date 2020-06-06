/**
 * Copyright (c) 2020 bluecub
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the BSD license. See LICENSE for details.
 **/

#include <math.h>
#include "ds_hash.h"


/* Compare Keys if match */ 
#define DS_CHK_KEYS_MATCH(node,k)\
    strncmp((((DS_Hash_Node_Ptr)(node))->key), (k),\
            strlen(((DS_Hash_Node_Ptr)(node))->key)) == 0\
            ? true : false


static inline unsigned ds_hash_hash (const char *key) {
    const char *lkey;
    unsigned hash;
    
    /* Hash the key by performing a number of bit operations on it */
    hash = 0;
    lkey = key;
    while (*lkey != '\0') {
        unsigned tmp;
        hash = (hash << 4) + (*lkey);

        if ((tmp = (hash & 0xf0000000))) {
            hash = hash ^ (tmp >> 24);
            hash = hash ^ tmp;
        }
        lkey++;
    }
    return hash;
}


static inline ushort ds_isprime (const unsigned num) {
    int factor;
    int sqrt_value;

    /* Special cases */
    if (num < 2)  
        return false; 
    if (num % 2 == 0) 
        return num == 2;

    sqrt_value = sqrt(num) + 1;
    for(factor = 3; factor < sqrt_value; factor+=2) {
        if(num % factor == 0) 
            return false;
    }
    return true;
}

static inline ushort ds_roundup_to_prime (const unsigned bsize) {
    unsigned psize = bsize; 
    if ((psize % 2) == 0) 
        psize++;

    while (!ds_isprime(psize))
        psize+=2;

    return psize;
}

static inline DS_Hash_Node_Ptr ds_create_hash_node (const char *key, const void *data, ushort *error_code) {
    DS_Hash_Node_Ptr new_node; 

    size_t keysize = strnlen(key, MAX_KEY_SIZE); 
    if (keysize > 0 && keysize < MAX_KEY_SIZE) {
        if ((new_node = (DS_Hash_Node_Ptr) malloc (sizeof(DS_Hash_Node))) == NULL) {
            *error_code = ds_error_cannot_allocate_memory;
            return NULL;
        }
        if ((new_node->key = (char *) malloc((keysize + 1) * sizeof(char))) == NULL) {
            *error_code = ds_error_cannot_allocate_memory;
            free (new_node);
            return NULL;
        }
        memcpy(new_node->key, key, keysize +1);
        new_node->key[keysize] = '\0';
        new_node->data = (void *)data;
        *error_code = ds_ok;
        return new_node;
    }
    *error_code = ds_error_in_key_size;
    return NULL;
} 

static ushort ds_hash_node_destroy (DS_Hash_Table_Ptr htbl, DS_Hash_Node_Ptr node, bool destroy_data) {
    if (destroy_data) {
        if (htbl->destroy == NULL)  
            return ds_error_callback_pointer_null;
        htbl->destroy (node->data);
    }
    memset(node->key, 0, strnlen(node->key, MAX_KEY_SIZE));
    free(node->key);
    memset(node, 0, sizeof(DS_Hash_Node));
    free(node);
    return ds_ok;
}

static ushort ds_hash_destroy (DS_Hash_Table_Ptr htbl) {
    ushort result = ds_ok;
    /* Destroy each bucket */
    for (int i = 0; i < htbl->buckets; i++) {
        /* Destroying bucket lists */
        DS_Hash_Node *hash_node;

        /* Remove each node */
        while (ds_list_size(&htbl->table[i]) > 0) {
            if (ds_list_rem_next(&htbl->table[i], NULL, (void **)&hash_node) == ds_ok) {
                result = ds_hash_node_destroy(htbl, hash_node, true);
                htbl->size--;
            }
        }
        memset(&htbl->table[i], 0, sizeof(DS_List));
    }

    htbl->buckets = 0;
    /* Free the storage allocated for the hash table */
    free(htbl->table);
    htbl->table = NULL;
    return result;
}

ushort ds_hash_init (DS_Hash_Table_Ptr *htbl, int buckets, Compare compare, Destroy destroy) {
    ushort buckets_number = buckets;

    if ((*htbl = (DS_Hash_Table_Ptr) malloc (sizeof(DS_Hash_Table))) == NULL) 
        return ds_error_cannot_allocate_memory;

    /* Hashing algorithm requires prime numbers */ 
    buckets_number = ds_roundup_to_prime(buckets_number);
    (*htbl)->buckets = buckets_number;

    /* Allocate space for the hashtable */
    if (((*htbl)->table = (DS_List *) malloc(buckets_number * sizeof(DS_List))) == NULL) {
        free(*htbl);
        return ds_error_cannot_allocate_memory;
    }


    /* Initialise the buckets */
    for (ushort i = 0; i < (*htbl)->buckets; i++) {
        (*htbl)->table[i].size = 0;
        (*htbl)->table[i].head = NULL;
        (*htbl)->table[i].tail = NULL;
        (*htbl)->table[i].destroy = NULL;
    }

    /* Encapsulate the functions */
    (*htbl)->hash = ds_hash_hash;
    (*htbl)->compare = compare;
    (*htbl)->destroy = destroy;

    /* Initialise the number of list_nodes in the table */
    (*htbl)->size = 0;
    return ds_ok;
}


ushort ds_hash_free (DS_Hash_Table_Ptr *htbl) {
    ushort retval;
    if ((retval = ds_hash_destroy(*htbl)) == ds_ok) {
        /* No operations are allowed now, but clear the structure as a precaution */
        memset(*htbl, 0, sizeof(DS_Hash_Table));
        free(*htbl);
        *htbl = NULL;
        return ds_ok;
    }
    return retval;
}


ushort ds_hash_put (const DS_Hash_Table_Ptr htbl, const char *key, const void *data) {
    void **temp = NULL;
    DS_Hash_Node *node;
    ushort retval;
    
    if (strnlen(key, MAX_KEY_SIZE) >= MAX_KEY_SIZE)
       return ds_error_in_key_size; 

    /* Check if the key exists or not */
    if (ds_hash_get(htbl, key, temp) == ds_ok)
        return ds_warning_key_exists;


    /* Insert the data into the bucket */
    ushort error_code;
    if ((node = ds_create_hash_node (key, data, &error_code)) == NULL)
        return error_code;

    /* Hash the key and determine the bucket index */
    int bucket = htbl->hash(key) % htbl->buckets;

    if ((retval = ds_list_add_head(&htbl->table[bucket], node)) == ds_ok)
        htbl->size++;

    return retval;
}

ushort ds_hash_remove (const DS_Hash_Table_Ptr htbl, const char *key, void **data) {
    DS_Node_Ptr list_node, prev_list_node = NULL;
    DS_Hash_Node_Ptr hash_node;

    if (!data)
        return ds_error_data_ptr_null;
    if (strnlen(key, MAX_KEY_SIZE) >= MAX_KEY_SIZE)
       return ds_error_in_key_size; 
    
    /* Check if hash table is empty */
    if (htbl->size == 0 || htbl->buckets == 0)
        return ds_warning_empty;

    /* Hash the key and determine the bucket index */
    int bucket = htbl->hash(key) % htbl->buckets;

    /* Search for the data in the bucket */
    for (list_node = ds_list_head(&htbl->table[bucket]); list_node != NULL; 
            list_node = ds_list_next(list_node)) {

        if (DS_CHK_KEYS_MATCH(list_node->data, key)) {
            ushort retval;
            /* Remove the data from the bucket */
            if ((retval = ds_list_rem_next(&htbl->table[bucket], prev_list_node, (void **)&hash_node)) == ds_ok) {
                /* Returning the data in the Hash Node */
                *data = hash_node->data;
                if ((retval = ds_hash_node_destroy(htbl, hash_node, false)) == ds_ok)  
                    /* Adjusting the size of the hash table */
                    htbl->size--;
                return retval;;
            } else {
                return retval;
            }
        }
        prev_list_node = list_node;
    }

    /* Return that the data not found */
    return ds_warning_element_not_found;
}

ushort ds_hash_get (const DS_Hash_Table_Ptr htbl, const char *key, void **data) {
    DS_Node_Ptr list_node;
    
    if(!data)
        return ds_error_data_ptr_null;
    if (strnlen(key, MAX_KEY_SIZE) >= MAX_KEY_SIZE)
       return ds_error_in_key_size; 
    
    /* Check if hash table is empty */
    if (htbl->size == 0 || htbl->buckets == 0)
        return ds_warning_empty;

    /* Hash the key and determine the bucket index*/
    int bucket = htbl->hash(key) % htbl->buckets;

    /* Search for the data in the bucket */
    for (list_node = ds_list_head(&htbl->table[bucket]); list_node != NULL; list_node =
            ds_list_next(list_node)) {

        if (DS_CHK_KEYS_MATCH(list_node->data, key)) {
            /* Pass back the data from the table */
            *data = ((DS_Hash_Node_Ptr)(list_node->data))->data;
            return ds_ok;
        }
    }
    /* Return that the data was not found */
    return ds_warning_element_not_found;
}


