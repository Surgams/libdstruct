/**
 * Copyright (c) 2020 bluecub
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the BSD license. See LICENSE for details.
 **/

#include <stdio.h>
#include <string.h>
#include "../src/ds_array_queue.h"
#include "../src/ds_common.h"
#include <stdlib.h>
#include <check.h>

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


/* Start Unit tests */

START_TEST (test_queue_init) {
    DS_A_Queue_Ptr queue;

    ds_array_queue_init(&queue, 10, (void *)destroy);

    ck_assert_int_eq(ds_array_queue_capacity(queue),  10);
    ck_assert_int_eq(ds_array_queue_used(queue), 0);
    ck_assert_int_eq(ds_array_queue_capacity(queue), 10);
    ck_assert_int_eq(queue->index, 0);
    ck_assert(ds_array_queue_capacity(queue) == queue->size);
    ck_assert_int_eq(ds_array_queue_free(&queue), ds_ok);
}
END_TEST

START_TEST (test_queue_enqueue_dequeue) {
    DS_A_Queue_Ptr queue;

    ds_array_queue_init(&queue, 4, (void *)destroy);

    ck_assert_int_eq(ds_array_queue_used(queue),  0);
    ck_assert_int_eq(ds_array_queue_capacity(queue),  4);
    Data *data_first, *data_forth, *data_third, *data_second;

    /* Preparing data_first */
    if ((data_first = (Data *) malloc(sizeof(Data))) == NULL) 
        ck_abort_msg("Couldn't allocate memory for data head, will stop test");

    /* Allocating data_first internal String */ 
    if ((data_first->string = (char *) malloc(10 * sizeof(char))) == NULL)
        ck_abort_msg("Couldn't allocate memory, will stop test");

    snprintf(data_first->string, 9, "first");
    data_first->position = 1;

    /*  Preparing data_second */
    if ((data_second = (Data *) malloc(sizeof(Data))) == NULL) 
        ck_abort_msg("Couldn't allocate memory for data tbd, will stop test");

    /* Allocating data_second internal String */ 
    if ((data_second->string = (char *) malloc(10 * sizeof(char))) == NULL)
        ck_abort_msg("Couldn't allocate memory, will stop test");

    snprintf(data_second->string, 9, "second");
    data_second->position = 2;

    /*  Preparing data_third */
    if ((data_third = (Data *) malloc(sizeof(Data))) == NULL) 
        ck_abort_msg("Couldn't allocate memory for data next, will stop test");

    /* Allocating data_first internal String */ 
    if ((data_third->string = (char *) malloc(10 * sizeof(char))) == NULL)
        ck_abort_msg("Couldn't allocate memory, will stop test");

    snprintf(data_third->string, 9, "third");
    data_third->position = 3;

    /*  Preparing data_forth */
    if ((data_forth = (Data *) malloc(sizeof(Data))) == NULL) 
        ck_abort_msg("Couldn't allocate memory for data tail, will stop test");

    /* Allocating data_first internal String */ 
    if ((data_forth->string = (char *) malloc(10 * sizeof(char))) == NULL)
        ck_abort_msg("Couldn't allocate memory, will stop test");

    snprintf(data_forth->string, 9, "forth");
    data_forth->position = 4;

    /* Checking entries correctness */
    ck_assert_str_eq(data_first->string, "first");
    ck_assert_str_eq(data_forth->string, "forth");
    ck_assert_str_eq(data_third->string, "third");
    ck_assert_str_eq(data_second->string, "second");

    /* queueing data in */
    ck_assert_int_eq(ds_array_queue_enqueue(queue, data_first), ds_ok);
    ck_assert_int_eq(ds_array_queue_enqueue(queue, data_second), ds_ok);
    ck_assert_int_eq(ds_array_queue_enqueue(queue, data_third), ds_ok);
    ck_assert_int_eq(ds_array_queue_enqueue(queue, data_forth), ds_ok);

    ck_assert_int_eq(ds_array_queue_used(queue),  4);
    
    
    ck_assert_int_eq(ds_array_queue_enqueue(queue, data_first), ds_warning_buffer_is_full);

    /* testing dequeue works as FIFO */
    Data *back_data;
    ck_assert_int_eq(ds_array_queue_dequeue(queue, (void **) &back_data), ds_ok);
    ck_assert_str_eq(back_data->string, "first");
    ck_assert_int_eq(back_data->position, 1);
    ck_assert_ptr_eq(data_first, back_data);
    
    ck_assert_int_eq(ds_array_queue_used(queue),  3);

    ck_assert_int_eq(ds_array_queue_dequeue(queue, (void **) &back_data), ds_ok);
    ck_assert_str_eq(back_data->string, "second");
    ck_assert_int_eq(back_data->position, 2);
    ck_assert_ptr_eq(data_second, back_data);
    ck_assert_int_eq(ds_array_queue_used(queue),  2);
    
    ck_assert_int_eq(ds_array_queue_dequeue(queue, (void **) &back_data), ds_ok);
    ck_assert_str_eq(back_data->string, "third");
    ck_assert_int_eq(back_data->position, 3);
    ck_assert_ptr_eq(data_third, back_data);
    ck_assert_int_eq(ds_array_queue_used(queue),  1);

    ck_assert_int_eq(ds_array_queue_dequeue(queue, (void **) &back_data), ds_ok);
    ck_assert_str_eq(back_data->string, "forth");
    ck_assert_int_eq(back_data->position, 4);
    ck_assert_ptr_eq(data_forth, back_data);
    ck_assert_int_eq(ds_array_queue_used(queue),  0);

    /* should return empty */
    ck_assert_int_eq(ds_array_queue_dequeue(queue, (void **) &back_data), ds_warning_empty);

    /* Push again to make sure it can accept data */
    ck_assert_int_eq(ds_array_queue_enqueue(queue, data_first), ds_ok);
    ck_assert_int_eq(ds_array_queue_enqueue(queue, data_second), ds_ok);
    ck_assert_int_eq(ds_array_queue_enqueue(queue, data_third), ds_ok);
    ck_assert_int_eq(ds_array_queue_enqueue(queue, data_forth), ds_ok);

    ck_assert_int_eq(ds_array_queue_used(queue),  4);
    
    ck_assert_int_eq(ds_array_queue_dequeue(queue, (void **) &back_data), ds_ok);
    ck_assert_str_eq(back_data->string, "first");
    ck_assert_int_eq(back_data->position, 1);
    ck_assert_ptr_eq(data_first, back_data);
    ck_assert_int_eq(ds_array_queue_used(queue),  3);
    destroy(back_data);    
    
    ck_assert_int_eq(ds_array_queue_dequeue(queue, (void **) &back_data), ds_ok);
    ck_assert_str_eq(back_data->string, "second");
    ck_assert_int_eq(back_data->position, 2);
    ck_assert_ptr_eq(data_second, back_data);
    ck_assert_int_eq(ds_array_queue_used(queue),  2);
    destroy(back_data);    
    
    ck_assert_int_eq(ds_array_queue_dequeue(queue, (void **) &back_data), ds_ok);
    ck_assert_str_eq(back_data->string, "third");
    ck_assert_int_eq(back_data->position, 3);
    ck_assert_ptr_eq(data_third, back_data);
    ck_assert_int_eq(ds_array_queue_used(queue),  1);
    destroy(back_data);    

    /* Preparing data_first for second round */
    if ((data_first = (Data *) malloc(sizeof(Data))) == NULL) 
        ck_abort_msg("Couldn't allocate memory for data head, will stop test");

    /* Allocating data_first internal String */ 
    if ((data_first->string = (char *) malloc(10 * sizeof(char))) == NULL)
        ck_abort_msg("Couldn't allocate memory, will stop test");

    snprintf(data_first->string, 9, "first");
    data_first->position = 1;
    ck_assert_int_eq(ds_array_queue_enqueue(queue, data_first), ds_ok);
    ck_assert_int_eq(ds_array_queue_used(queue),  2);
    
    ck_assert_int_eq(ds_array_queue_dequeue(queue, (void **) &back_data), ds_ok);
    ck_assert_str_eq(back_data->string, "forth");
    ck_assert_int_eq(back_data->position, 4);
    ck_assert_ptr_eq(data_forth, back_data);
    ck_assert_int_eq(ds_array_queue_used(queue),  1);
    destroy(back_data);    
    
    ck_assert_int_eq(ds_array_queue_dequeue(queue, (void **) &back_data), ds_ok);
    ck_assert_str_eq(back_data->string, "first");
    ck_assert_int_eq(back_data->position, 1);
    ck_assert_ptr_eq(data_first, back_data);
    ck_assert_int_eq(ds_array_queue_used(queue),  0);
    destroy(back_data);    

    /* free and exit */
    ck_assert_int_eq(ds_array_queue_free(&queue), ds_ok);
}
END_TEST

START_TEST (test_queue_purge) {
    DS_A_Queue_Ptr queue;

    ds_array_queue_init(&queue, 4, (void *)destroy);

    ck_assert_int_eq(ds_array_queue_used(queue),  0);
    ck_assert_int_eq(ds_array_queue_capacity(queue),  4);
    Data *data_first, *data_forth, *data_third, *data_second;

    /* Preparing data_first */
    if ((data_first = (Data *) malloc(sizeof(Data))) == NULL) 
        ck_abort_msg("Couldn't allocate memory for data head, will stop test");

    /* Allocating data_first internal String */ 
    if ((data_first->string = (char *) malloc(10 * sizeof(char))) == NULL)
        ck_abort_msg("Couldn't allocate memory, will stop test");

    snprintf(data_first->string, 9, "first");
    data_first->position = 1;


    /*  Preparing data_forth */
    if ((data_forth = (Data *) malloc(sizeof(Data))) == NULL) 
        ck_abort_msg("Couldn't allocate memory for data tail, will stop test");

    /* Allocating data_forth internal String */ 
    if ((data_forth->string = (char *) malloc(10 * sizeof(char))) == NULL)
        ck_abort_msg("Couldn't allocate memory, will stop test");

    snprintf(data_forth->string, 9, "forth");
    data_forth->position = 4;

    /*  Preparing data_third */
    if ((data_third = (Data *) malloc(sizeof(Data))) == NULL) 
        ck_abort_msg("Couldn't allocate memory for data next, will stop test");

    /* Allocating data_third internal String */ 
    if ((data_third->string = (char *) malloc(10 * sizeof(char))) == NULL)
        ck_abort_msg("Couldn't allocate memory, will stop test");

    snprintf(data_third->string, 9, "third");
    data_third->position = 3;

    /*  Preparing data_second */
    if ((data_second = (Data *) malloc(sizeof(Data))) == NULL) 
        ck_abort_msg("Couldn't allocate memory for data tbd, will stop test");

    /* Allocating data_second internal String */ 
    if ((data_second->string = (char *) malloc(10 * sizeof(char))) == NULL)
        ck_abort_msg("Couldn't allocate memory, will stop test");

    snprintf(data_second->string, 9, "second");
    data_second->position = 2;


    ck_assert_str_eq(data_first->string, "first");
    ck_assert_str_eq(data_forth->string, "forth");
    ck_assert_str_eq(data_third->string, "third");
    ck_assert_str_eq(data_second->string, "second");
    
    ck_assert_int_eq(ds_array_queue_enqueue(queue, data_first), ds_ok);
    ck_assert_int_eq(ds_array_queue_enqueue(queue, data_second), ds_ok);
    ck_assert_int_eq(ds_array_queue_enqueue(queue, data_third), ds_ok);
    ck_assert_int_eq(ds_array_queue_enqueue(queue, data_forth), ds_ok);
    
    ck_assert_int_eq(ds_array_queue_used(queue),  4);

    ck_assert_int_eq(ds_array_queue_purge(queue), ds_ok);
    
    ck_assert_int_eq(ds_array_queue_capacity(queue),  4);
    ck_assert_int_eq(ds_array_queue_used(queue), 0);
    ck_assert_int_eq(ds_array_queue_capacity(queue), 4);
    ck_assert_int_eq(queue->index, 0);
    ck_assert(ds_array_queue_capacity(queue) == queue->size);
}
END_TEST

Suite * queue_suite(void) {
    Suite *s;
    TCase *tc_core;

    s = suite_create("DS Array Queue");

    /* Core test case */
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_queue_init);
    tcase_add_test(tc_core, test_queue_enqueue_dequeue);
    tcase_add_test(tc_core, test_queue_purge);
    suite_add_tcase(s, tc_core);

    return s;
}

int main(void) {
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = queue_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
