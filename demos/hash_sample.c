/**
 * Copyright (c) 2020 bluecub
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the BSD license. See LICENSE for details.
 **/

#include <stdio.h>
#include <string.h>
#include "../src/ds_common.h"
#include "../src/ds_hash.h"
#include <stdlib.h>

/* Sample client data type and destro */
typedef struct data {
    int size;
    char* string;
} Data;
typedef Data * Data_Ptr;

static int destroy (Data *data) {
    if (data != NULL) {
        if (data->string != NULL) {
            free(data->string);
            data->string = NULL;
        }
        free(data);
        return EXIT_SUCCESS;
    } 
    return EXIT_FAILURE;
}

int main() {
    unsigned short ret_code;

    /* Create new data */
    Data_Ptr data1 = (Data_Ptr) malloc(sizeof(Data));
    data1->size = 10; /* any number */
    data1->string = (char *) malloc(6 * sizeof(char));
    memcpy(data1->string, "first", 6);
    data1->string[5] = '\0';

    char some_key1[75] = {'\0'};
    memcpy(some_key1, "key1", 75);

    Data_Ptr data2 = (Data_Ptr) malloc(sizeof(Data));
    data2->size = 12; /* any number */
    data2->string = (char *) malloc(7 * sizeof(char));
    memcpy(data2->string, "second", 7);
    data2->string[6] = '\0';

    char some_key2[75] = {'\0'};
    memcpy(some_key2, "key2", 75);

    /* Create hash table */ 
    DS_Hash_Table_Ptr htbl;
    int bucket_number = 22;
    if ((ret_code = ds_hash_init (&htbl, bucket_number, NULL, (void *)destroy)) != ds_ok)
        printf("\n%s\n", ERROR_MESSAGE(ret_code));
   
    /* Put element to the table */
    if ((ret_code = ds_hash_put(htbl, some_key1, data1)) != ds_ok) 
        printf("\n%s\n", ERROR_MESSAGE(ret_code));
    if ((ret_code = ds_hash_put(htbl, some_key2, data2)) != ds_ok)
        printf("\n%s\n", ERROR_MESSAGE(ret_code));
   
    /* Check if the data exists now */
    Data_Ptr data_back1;
    if ((ret_code = ds_hash_get(htbl, some_key1, (void **)&data_back1)) != ds_ok)
        printf("\n%s\n", ERROR_MESSAGE(ret_code));
    printf("\nReturned data is [%s] and it should be [%s]", data_back1->string, data1->string);
    
    Data_Ptr data_back2;
    if ((ret_code = ds_hash_get(htbl, some_key2, (void **)&data_back2)) != ds_ok)
        printf("\n%s\n", ERROR_MESSAGE(ret_code));
    printf("\nReturned data is [%s] and it should be [%s]", data_back2->string, data2->string);
    
    /* Remove the first node */
    if ((ret_code = ds_hash_remove (htbl, some_key1, (void **)&data_back1)) != ds_ok)
        printf("\n%s\n", ERROR_MESSAGE(ret_code));
    printf("\nRemoved string is [%s] and it should be equal to [%s]", data_back1->string, data1->string);
    

    if ((ret_code = ds_hash_free (&htbl)) != ds_ok)
        printf("\n%s\n", ERROR_MESSAGE(ret_code));
    return 0;
}
