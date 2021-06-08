/**
 * Copyright (c) 2020 Surgams
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the BSD license. See LICENSE for details.
 **/

#include <stdio.h>
#include <string.h>
#include "../src/ds_array_stack.h"
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

void copmare_int(int a, int b) {
    if(a == b)
        printf("\ncompare: values match");
    else
        printf("\ncompare: values don't match (%d != %d)", a, b);
}


void copmare_str(char* a, char* b) {
    if(strcmp(a, b) == 0)
        printf("\ncompare: values match");
    else
        printf("\ncompare: values don't match (%s != %s)", a, b);
}


int main() {
    DS_A_Stack_Ptr stack;

    ds_array_stack_init(&stack, 10, (void *)destroy);

    copmare_int(ds_array_stack_size(stack),  0);
    copmare_int(ds_array_stack_capacity(stack),  10);
    Data *data_first, *data_forth, *data_third, *data_second;

    /* Preparing data_first */
    if ((data_first = (Data *) malloc(sizeof(Data))) == NULL) {
        printf("\nCouldn't allocate memory for data head, will stop test");
    }

    /* Allocating data_first internal String */ 
    if ((data_first->string = (char *) malloc(10 * sizeof(char))) == NULL)
        printf("\nCouldn't allocate memory, will stop test");

    snprintf(data_first->string, 9, "first");
    data_first->position = 1;


    /*  Preparing data_forth */
    if ((data_forth = (Data *) malloc(sizeof(Data))) == NULL) {
        printf("\nCouldn't allocate memory for data tail, will stop test");
    }

    /* Allocating data_first internal String */ 
    if ((data_forth->string = (char *) malloc(10 * sizeof(char))) == NULL)
        printf("\nCouldn't allocate memory, will stop test");

    snprintf(data_forth->string, 9, "forth");
    data_forth->position = 4;

    /*  Preparing data_third */
    if ((data_third = (Data *) malloc(sizeof(Data))) == NULL) {
        printf("\nCouldn't allocate memory for data next, will stop test");
    }

    /* Allocating data_first internal String */ 
    if ((data_third->string = (char *) malloc(10 * sizeof(char))) == NULL)
        printf("\nCouldn't allocate memory, will stop test");

    snprintf(data_third->string, 9, "third");
    data_third->position = 3;

    /*  Preparing data_second */
    if ((data_second = (Data *) malloc(sizeof(Data))) == NULL) {
        printf("\nCouldn't allocate memory for data tbd, will stop test");
    }

    /* Allocating data_second internal String */ 
    if ((data_second->string = (char *) malloc(10 * sizeof(char))) == NULL)
        printf("\nCouldn't allocate memory, will stop test");

    snprintf(data_second->string, 9, "second");
    data_second->position = 2;

    
    printf("\nPushing now");
    copmare_int(ds_array_stack_push(stack, data_first), ds_ok);
    copmare_int(ds_array_stack_push(stack, data_second), ds_ok);
    copmare_int(ds_array_stack_push(stack, data_third), ds_ok);
    copmare_int(ds_array_stack_push(stack, data_forth), ds_ok);

    printf("\nChecking stack size now");
    copmare_int(ds_array_stack_size(stack),  4);
    
    /* testing pop works as LIFO */
    Data *back_data;
    copmare_int(ds_array_stack_pop(stack, (void **) &back_data), ds_ok);
    copmare_str(back_data->string, "forth");
    copmare_int(ds_array_stack_size(stack),  3);
    destroy(back_data);    

    copmare_int(ds_array_stack_pop(stack, (void **) &back_data), ds_ok);
    copmare_str(back_data->string, "third");
    copmare_int(ds_array_stack_size(stack),  2);
    destroy(back_data);    

    copmare_int(ds_array_stack_pop(stack, (void **) &back_data), ds_ok);
    copmare_str(back_data->string, "second");
    copmare_int(ds_array_stack_size(stack),  1);
    destroy(back_data);    
    
    copmare_int(ds_array_stack_pop(stack, (void **) &back_data), ds_ok);
    copmare_str(back_data->string, "first");
    copmare_int(ds_array_stack_size(stack),  0);
    destroy(back_data);    

    /* free and exit */
    copmare_int(ds_array_stack_free(&stack), ds_ok);
    printf("\n");
}
