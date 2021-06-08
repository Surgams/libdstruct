/**
*  Copyright (c) 2020 Surgams
*
*  This library is free software; you can redistribute it and/or modify it
*  under the terms of the BSD license. See LICENSE for details.
*
**/

#include <string.h>
#include "../src/ds_set.h"
#include "../src/ds_common.h"
#include <stdlib.h>
#include <stdio.h>

#define MAX_STR_SIZE 75

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

static int compare (Data *data1, Data *data2) {
    if (data1->size > data2->size)
        return 1;
    else if (data1->size < data2->size)
        return -1;
    else 
        return strncmp(data1->string, data2->string, strnlen(data1->string, MAX_STR_SIZE));
}

int main() {
    DS_Set_Ptr set1, set2, setu = NULL;

    ds_set_init(&set1, (void *)compare, (void *)destroy);
    ds_set_init(&set2, (void *)compare, (void *)destroy);

    Data *data, *data2;

    uint8_t ret_code =-1;

    /*Initialise data*/
    if ((data = (Data *) malloc(sizeof(Data))) == NULL) 
        return -1;

    data->size = 10;
    
    if ((data->string = (char *) malloc((data->size + 1) * sizeof(char))) == NULL)
       return -1;

    snprintf(data->string, 7, "%s", "test");

    /* Insert data into s1 */
    if ((ret_code = ds_set_insert(set1, data)) != ds_ok)
       printf("\n%s\n", ERROR_MESSAGE(ret_code));

    ds_set_size(set1);

    /* Initialise data2 */
    if ((data2 = (Data *) malloc(sizeof(Data))) == NULL) 
        return -1;

    data2->size = 12;
    if ((data2->string = (char *) malloc((data2->size)  * sizeof(char))) == NULL)
        return -1;
    
    snprintf(data2->string, 7, "%s", "test2");

    /* Insert data and data2 into set2 */
    
    if ((ret_code = ds_set_insert(set2, data2)) != ds_ok) 
       printf("\n%s\n", ERROR_MESSAGE(ret_code));
    
    if ((ret_code = ds_set_insert(set2, data)) != ds_ok)
        printf("\n%s\n", ERROR_MESSAGE(ret_code));
    
    printf("\nset2 size: %d",ds_set_size(set2));
    
    /* Check equal and subset */
    ds_set_diff(&setu, set2, set1);
    printf("\n-->setu size: %d ",ds_set_size(setu));
    

    ds_set_free(&set1);
    ds_set_free(&set2);
    ds_set_free(&setu);
}
