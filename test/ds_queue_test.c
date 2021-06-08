/**
*  Copyright (c) 2020 Surgams
*
*  This library is free software; you can redistribute it and/or modify it
*  under the terms of the BSD license. See LICENSE for details.
*
**/

#include <string.h>
#include "../src/ds_queue.h"
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
    DS_Queue_Ptr queue;

    ds_queue_init(&queue, (void *)destroy);

    ck_assert_int_eq(ds_queue_size(queue),  0);
    ck_assert_int_eq(ds_queue_free(&queue), ds_ok);
}
END_TEST

START_TEST (test_queue_enqueue_dequeue) {
    DS_Queue_Ptr queue;

    ds_queue_init(&queue, (void *)destroy);

    ck_assert_int_eq(ds_queue_size(queue),  0);
    Data *data_first, *data_forth, *data_third, *data_second;

    /* Preparing data_first */
    if ((data_first = (Data *) malloc(sizeof(Data))) == NULL) {
        ck_abort_msg("Couldn't allocate memory for data head, will stop test");
    }

    /* Allocating data_first internal String */ 
    if ((data_first->string = (char *) malloc(10 * sizeof(char))) == NULL)
        ck_abort_msg("Couldn't allocate memory, will stop test");

    memcpy(data_first->string, "first", 9);
    data_first->position = 1;


    /*  Preparing data_forth */
    if ((data_forth = (Data *) malloc(sizeof(Data))) == NULL) {
        ck_abort_msg("Couldn't allocate memory for data tail, will stop test");
    }

    /* Allocating data_first internal String */ 
    if ((data_forth->string = (char *) malloc(10 * sizeof(char))) == NULL)
        ck_abort_msg("Couldn't allocate memory, will stop test");

    memcpy(data_forth->string, "forth", 9);
    data_forth->position = 4;

    /*  Preparing data_third */
    if ((data_third = (Data *) malloc(sizeof(Data))) == NULL) {
        ck_abort_msg("Couldn't allocate memory for data next, will stop test");
    }

    /* Allocating data_first internal String */ 
    if ((data_third->string = (char *) malloc(10 * sizeof(char))) == NULL)
        ck_abort_msg("Couldn't allocate memory, will stop test");

    memcpy(data_third->string, "third", 9);
    data_third->position = 3;

    /*  Preparing data_second */
    if ((data_second = (Data *) malloc(sizeof(Data))) == NULL) {
        ck_abort_msg("Couldn't allocate memory for data tbd, will stop test");
    }

    /* Allocating data_second internal String */ 
    if ((data_second->string = (char *) malloc(10 * sizeof(char))) == NULL)
        ck_abort_msg("Couldn't allocate memory, will stop test");

    memcpy(data_second->string, "second", 9);
    data_second->position = 2;


    ck_assert_str_eq(data_first->string, "first");
    ck_assert_str_eq(data_forth->string, "forth");
    ck_assert_str_eq(data_third->string, "third");
    ck_assert_str_eq(data_second->string, "second");

    ck_assert_int_eq(ds_queue_enqueue(queue, data_first), 0);
    ck_assert_int_eq(ds_queue_enqueue(queue, data_second), 0);
    ck_assert_int_eq(ds_queue_enqueue(queue, data_third), 0);
    ck_assert_int_eq(ds_queue_enqueue(queue, data_forth), 0);

    ck_assert_int_eq(ds_queue_size(queue),  4);

    /* Testing Peek*/
    char *peek_str = ((Data *)(ds_queue_peek(queue)))->string;
    ck_assert_str_eq(peek_str, "first");


    /* testing dequeue works as FIFO */
    Data *back_data;
    ck_assert_int_eq(ds_queue_dequeue(queue, (void **) &back_data), 0);
    ck_assert_str_eq(back_data->string, "first");
    ck_assert_int_eq(back_data->position, 1);
    ck_assert_ptr_eq(data_first, back_data);
    ck_assert_int_eq(ds_queue_size(queue),  3);

    ck_assert_int_eq(ds_queue_dequeue(queue, (void **) &back_data), 0);
    ck_assert_str_eq(back_data->string, "second");
    ck_assert_int_eq(back_data->position, 2);
    ck_assert_ptr_eq(data_second, back_data);
    ck_assert_int_eq(ds_queue_size(queue),  2);

    ck_assert_int_eq(ds_queue_dequeue(queue, (void **) &back_data), 0);
    ck_assert_str_eq(back_data->string, "third");
    ck_assert_int_eq(back_data->position, 3);
    ck_assert_ptr_eq(data_third, back_data);
    ck_assert_int_eq(ds_queue_size(queue),  1);
    
    ck_assert_int_eq(ds_queue_dequeue(queue, (void **) &back_data), 0);
    ck_assert_str_eq(back_data->string, "forth");
    ck_assert_int_eq(back_data->position, 4);
    ck_assert_ptr_eq(data_forth, back_data);
    ck_assert_int_eq(ds_queue_size(queue),  0);
    

    /* free and exit */
    ck_assert_int_eq(ds_queue_free(&queue), ds_ok);
}
END_TEST

START_TEST (test_queue_purge) {
    DS_Queue_Ptr queue;

    ds_queue_init(&queue, (void *)destroy);

    ck_assert_int_eq(ds_queue_size(queue),  0);
    Data *data_first, *data_forth, *data_third, *data_second;

    /* Preparing data_first */
    if ((data_first = (Data *) malloc(sizeof(Data))) == NULL) {
        ck_abort_msg("Couldn't allocate memory for data head, will stop test");
    }

    /* Allocating data_first internal String */ 
    if ((data_first->string = (char *) malloc(10 * sizeof(char))) == NULL)
        ck_abort_msg("Couldn't allocate memory, will stop test");

    memcpy(data_first->string, "first", 9);
    data_first->position = 1;


    /*  Preparing data_forth */
    if ((data_forth = (Data *) malloc(sizeof(Data))) == NULL) {
        ck_abort_msg("Couldn't allocate memory for data tail, will stop test");
    }

    /* Allocating data_first internal String */ 
    if ((data_forth->string = (char *) malloc(10 * sizeof(char))) == NULL)
        ck_abort_msg("Couldn't allocate memory, will stop test");

    memcpy(data_forth->string, "forth", 9);
    data_forth->position = 4;

    /*  Preparing data_third */
    if ((data_third = (Data *) malloc(sizeof(Data))) == NULL) {
        ck_abort_msg("Couldn't allocate memory for data next, will stop test");
    }

    /* Allocating data_first internal String */ 
    if ((data_third->string = (char *) malloc(10 * sizeof(char))) == NULL)
        ck_abort_msg("Couldn't allocate memory, will stop test");

    memcpy(data_third->string, "third", 9);
    data_third->position = 3;

    /*  Preparing data_second */
    if ((data_second = (Data *) malloc(sizeof(Data))) == NULL) {
        ck_abort_msg("Couldn't allocate memory for data tbd, will stop test");
    }

    /* Allocating data_second internal String */ 
    if ((data_second->string = (char *) malloc(10 * sizeof(char))) == NULL)
        ck_abort_msg("Couldn't allocate memory, will stop test");

    memcpy(data_second->string, "second", 9);
    data_second->position = 2;


    ck_assert_str_eq(data_first->string, "first");
    ck_assert_str_eq(data_forth->string, "forth");
    ck_assert_str_eq(data_third->string, "third");
    ck_assert_str_eq(data_second->string, "second");

    ck_assert_int_eq(ds_queue_enqueue(queue, data_first), 0);
    ck_assert_int_eq(ds_queue_enqueue(queue, data_second), 0);
    ck_assert_int_eq(ds_queue_enqueue(queue, data_third), 0);
    ck_assert_int_eq(ds_queue_enqueue(queue, data_forth), 0);

    ck_assert_int_eq(ds_queue_size(queue),  4);
    
    /* Testing Peek*/
    char *peek_str = ((Data *)(ds_queue_peek(queue)))->string;
    ck_assert_str_eq(peek_str, "first");

    ck_assert_int_eq(ds_queue_purge(queue), 0);
    ck_assert_int_eq(ds_queue_size(queue),  0);
    
    /* free and exit */
    ck_assert_int_eq(ds_queue_free(&queue), ds_ok);
}
END_TEST



Suite * queue_suite(void) {
    Suite *s;
    TCase *tc_core;

    s = suite_create("DS Queue");

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
