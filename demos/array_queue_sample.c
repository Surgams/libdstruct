#include <stdio.h>
#include <string.h>
#include "../src/ds_array_queue.h"
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
    DS_A_Queue_Ptr queue;

    ds_array_queue_init(&queue, 10, (void *)destroy);

    copmare_int(ds_array_queue_used(queue),  0);
    copmare_int(ds_array_queue_capacity(queue),  10);
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

    /* Allocating data_forth internal String */ 
    if ((data_forth->string = (char *) malloc(10 * sizeof(char))) == NULL)
        printf("\nCouldn't allocate memory, will stop test");

    snprintf(data_forth->string, 9, "forth");
    data_forth->position = 4;

    /*  Preparing data_third */
    if ((data_third = (Data *) malloc(sizeof(Data))) == NULL) {
        printf("\nCouldn't allocate memory for data next, will stop test");
    }

    /* Allocating data_third internal String */ 
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
    copmare_int(ds_array_queue_enqueue(queue, data_first), ds_ok);
    copmare_int(ds_array_queue_enqueue(queue, data_second), ds_ok);
    copmare_int(ds_array_queue_enqueue(queue, data_third), ds_ok);
    copmare_int(ds_array_queue_enqueue(queue, data_forth), ds_ok);

    printf("\nChecking queue size now");
    copmare_int(ds_array_queue_used(queue),  4);

    /* testing dequeue works as LIFO */
    Data *back_data;
    copmare_int(ds_array_queue_dequeue(queue, (void **) &back_data), ds_ok);
    copmare_str(back_data->string, "first");
    copmare_int(ds_array_queue_used(queue),  3);
    destroy(back_data);    

    copmare_int(ds_array_queue_dequeue(queue, (void **) &back_data), ds_ok);
    copmare_str(back_data->string, "second");
    copmare_int(ds_array_queue_used(queue),  2);
    destroy(back_data);    

    copmare_int(ds_array_queue_dequeue(queue, (void **) &back_data), ds_ok);
    copmare_str(back_data->string, "third");
    copmare_int(ds_array_queue_used(queue),  1);
    destroy(back_data);    

    copmare_int(ds_array_queue_dequeue(queue, (void **) &back_data), ds_ok);
    copmare_str(back_data->string, "forth");
    copmare_int(ds_array_queue_used(queue),  0);
    destroy(back_data);    

    /* free and exit */
    copmare_int(ds_array_queue_free(&queue), ds_ok);
    printf("\n");
}
