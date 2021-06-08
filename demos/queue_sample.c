/**
 * Copyright (c) 2020 Surgams
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the BSD license. See LICENSE for details.
 **/

#include <stdio.h>
#include <string.h>
#include "../src/ds_queue.h"
#include "../src/ds_common.h"
#include <stdlib.h>

/* Sample client data type and destro */
typedef struct data {
    int position;
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
    DS_Queue_Ptr queue;
    unsigned short ret_code;
    ds_queue_init(&queue, (void *)destroy);

    Data *data_first, *data_forth, *data_third, *data_second;

    /* Preparing data_first */
    if ((data_first = (Data *) malloc(sizeof(Data))) == NULL) {
        printf("\nCouldn't allocate memory for data head, will stop test");
    }

    /* Allocating data_first internal String */ 
    if ((data_first->string = (char *) malloc(10 * sizeof(char))) == NULL)
        printf("\nCouldn't allocate memory, will stop test");

    memcpy(data_first->string, "first", 9);
    data_first->string[9] = '\0';
    data_first->position = 1;


    /*  Preparing data_forth */
    if ((data_forth = (Data *) malloc(sizeof(Data))) == NULL) {
        printf("\nCouldn't allocate memory for data tail, will stop test");
    }

    /* Allocating data_first internal String */ 
    if ((data_forth->string = (char *) malloc(10 * sizeof(char))) == NULL)
        printf("\nCouldn't allocate memory, will stop test");

    memcpy(data_forth->string, "forth", 9);
    data_forth->string[9] = '\0';
    data_forth->position = 4;

    /*  Preparing data_third */
    if ((data_third = (Data *) malloc(sizeof(Data))) == NULL) {
        printf("\nCouldn't allocate memory for data next, will stop test");
    }

    /* Allocating data_first internal String */ 
    if ((data_third->string = (char *) malloc(10 * sizeof(char))) == NULL)
        printf("\nCouldn't allocate memory, will stop test");

    memcpy(data_third->string, "third", 9);
    data_third->string[9] = '\0';
    data_third->position = 3;

    /*  Preparing data_second */
    if ((data_second = (Data *) malloc(sizeof(Data))) == NULL) {
        printf("\nCouldn't allocate memory for data tbd, will stop test");
    }

    /* Allocating data_second internal String */ 
    if ((data_second->string = (char *) malloc(10 * sizeof(char))) == NULL)
        printf("\nCouldn't allocate memory, will stop test");

    memcpy(data_second->string, "second", 9);
    data_second->string[9] = '\0';
    data_second->position = 2;

    if ((ret_code = ds_queue_enqueue(queue, data_first)) != ds_ok)
        printf("\n%s\n", ERROR_MESSAGE(ret_code));
    if ((ret_code = ds_queue_enqueue(queue, data_second)) != ds_ok)
        printf("\n%s\n", ERROR_MESSAGE(ret_code));
    if ((ret_code = ds_queue_enqueue(queue, data_third)) != ds_ok)
        printf("\n%s\n", ERROR_MESSAGE(ret_code));
    if ((ret_code = ds_queue_enqueue(queue, data_forth)) != ds_ok)
        printf("\n%s\n", ERROR_MESSAGE(ret_code));

    printf("\nSize of the queue is: %d and it should be 4", ds_queue_size(queue));

    char *peek_str = ((Data *)(ds_queue_peek(queue)))->string;
    printf("\nThe peek returned stirng is %s and must be forth", peek_str);


    /* testing dequeue works as FIFO */
    Data *back_data;
    if ((ret_code = ds_queue_dequeue(queue, (void **) &back_data)) != ds_ok)
        printf("\n%s\n", ERROR_MESSAGE(ret_code));
    printf("\n Print poped string: %s", (back_data->string));
    printf("\n Print poped position: %d" ,(back_data->position));
    destroy(back_data);    

    if ((ret_code = ds_queue_dequeue(queue, (void **) &back_data)) != ds_ok)
        printf("\n%s\n", ERROR_MESSAGE(ret_code));
    destroy(back_data);    

    if ((ret_code = ds_queue_dequeue(queue, (void **) &back_data)) != ds_ok)
        printf("\n%s\n", ERROR_MESSAGE(ret_code));
    destroy(back_data);    
    
    if ((ret_code = ds_queue_dequeue(queue, (void **) &back_data)) != ds_ok)
        printf("\n%s\n", ERROR_MESSAGE(ret_code));
    destroy(back_data);    
    
    printf("\nSize of the queue is: %d and it should be 0", ds_queue_size(queue));

    /* free and exit */
    if ((ret_code = ds_queue_free(&queue)) !=  ds_ok)
        printf("\n%s\n", ERROR_MESSAGE(ret_code));
    return 0;
}

