/**
 * Copyright (c) 2020 bluecub
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the BSD license. See LICENSE for details.
 **/

#include <stdio.h>
#include "../src/ds_list.h"
#include "../src/ds_common.h"

/* Sample client data type and destro */
typedef struct data {
    int size;
    char* string;
} Data;

static int destroy (Data *data){
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

    DS_List_Ptr list;

    ds_list_init(&list, (void *)destroy);

    Data *data_head, *data_tail, *data_next, *data_tbdl;

    /* Preparing data_head */
    if ((data_head = (Data *) malloc(sizeof(Data))) == NULL) 
        return -1;

    /* Allocating data_head internal String */ 
    if ((data_head->string = (char *) malloc(6 * sizeof(char))) == NULL)
        return -1;

    memcpy(data_head->string, "head", 5);
    data_head->string[5] = '\0';
    data_head->size = 10;


    /*  Preparing data_tail */
    if ((data_tail = (Data *) malloc(sizeof(Data))) == NULL) 
        return -1;

    /* Allocating data_head internal String */ 
    if ((data_tail->string = (char *) malloc(6 * sizeof(char))) == NULL)
        return -1;

    memcpy(data_tail->string, "tail", 5);
    data_tail->string[5] = '\0';
    data_tail->size = 5;

    /*  Preparing data_next */
    if ((data_next = (Data *) malloc(sizeof(Data))) == NULL) 
        return -1;

    /* Allocating data_head internal String */ 
    if ((data_next->string = (char *) malloc(6 * sizeof(char))) == NULL)
        return -1;

    memcpy(data_next->string, "next", 5);
    data_next->string[5] = '\0';
    data_next->size = 3;

    /*  Preparing 'data to be deleted' data_tbdl */
    if ((data_tbdl = (Data *) malloc(sizeof(Data))) == NULL) {
        printf("\nCouldn't allocate memory for data tbd, will stop test");
    }

    /* Allocating data_head internal String */ 
    if ((data_tbdl->string = (char *) malloc(6 * sizeof(char))) == NULL)
        printf("\nCouldn't allocate memory, will stop test");

    memcpy(data_tbdl->string, "tbdl", 5);
    data_tbdl->string[5] = '\0';
    data_tbdl->size = 12;


    if ((ret_code = ds_list_add_head(list, data_head)) != ds_ok)
        printf("\n%s\n", ERROR_MESSAGE(ret_code));
    if ((ret_code = ds_list_add_next(list, list->head, data_tbdl)) != ds_ok)
        printf("\n%s\n", ERROR_MESSAGE(ret_code));
    if ((ret_code = ds_list_add_next(list, list->head->next, data_next)) != ds_ok)
        printf("\n%s\n", ERROR_MESSAGE(ret_code));
    if ((ret_code = ds_list_add_tail(list, data_tail)) != ds_ok)
        printf("\n%s\n", ERROR_MESSAGE(ret_code));

    printf("\nThe size after adding element is %d and it should be 4", ds_list_size(list));

    Data *deleted_data;
    /* Deleting the tbdl data, which is the node after header */
    if ((ret_code = ds_list_rem_next(list, ds_list_head(list), (void **)&deleted_data)) != ds_ok)
        printf("\n%s\n", ERROR_MESSAGE(ret_code));

    printf("\nNew size is %d", ds_list_size(list));
    destroy(deleted_data);

    /* free and exit */
    if ((ret_code = ds_list_free(&list)) !=  ds_ok)
        printf("\n%s\n", ERROR_MESSAGE(ret_code));

    return 0;
}
