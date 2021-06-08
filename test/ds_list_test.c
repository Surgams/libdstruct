/**
 * Copyright (c) 2020 Surgams
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the BSD license. See LICENSE for details.
 **/

#include <string.h>
#include "../src/ds_list.h"
#include "../src/ds_common.h"
#include <stdlib.h>
#include <check.h>

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

/* Start Unit tests */

START_TEST (test_list_init) {
    DS_List_Ptr list;

    ds_list_init(&list, (void *)destroy);

    ck_assert_int_eq(ds_list_size(list),  0);
    ck_assert_int_eq(ds_list_free(&list), ds_ok);
}

END_TEST

START_TEST (test_list_add_header) {
    DS_List_Ptr list;

    ds_list_init(&list, (void *)destroy);

    ck_assert_int_eq(ds_list_size(list),  0);
    Data *data;

    if ((data = (Data *) malloc(sizeof(Data))) == NULL) {
        ck_abort_msg("Couldn't allocate memory, will stop test");
    }

    if ((data->string = (char *) malloc((data->size + 1) * sizeof(char))) == NULL)
        ck_abort_msg("Couldn't allocate memory, will stop test");

    memcpy(data->string, "test", 5);
    data->size = 10;
    ck_assert_str_eq(data->string, "test");

    int result = ds_list_add_head(list, data);
    ck_assert_int_eq(result, 0); 
    ck_assert_int_eq(ds_list_size(list),  1);
    
    Data * back_data = (Data *) ds_list_head(list)->data;
    ck_assert_str_eq(back_data->string, "test");

    ck_assert_int_eq(ds_list_free(&list), ds_ok);

}
END_TEST

START_TEST (test_list_add_tail) {
    DS_List_Ptr list;

    ds_list_init(&list, (void *)destroy);

    ck_assert_int_eq(ds_list_size(list),  0);
    Data *data_head, *data_tail;

    /* Preparing data_head */
    if ((data_head = (Data *) malloc(sizeof(Data))) == NULL) {
        ck_abort_msg("Couldn't allocate memory for data head, will stop test");
    }

    /* Allocating data_head internal String */ 
    if ((data_head->string = (char *) malloc(6 * sizeof(char))) == NULL)
        ck_abort_msg("Couldn't allocate memory, will stop test");

    memcpy(data_head->string, "head", 5);
    data_head->size = 10;


    /*  Preparing data_tail */
    if ((data_tail = (Data *) malloc(sizeof(Data))) == NULL) {
        ck_abort_msg("Couldn't allocate memory for data tail, will stop test");
    }

    /* Allocating data_head internal String */ 
    if ((data_tail->string = (char *) malloc(6 * sizeof(char))) == NULL)
        ck_abort_msg("Couldn't allocate memory, will stop test");

    memcpy(data_tail->string, "tail", 5);
    data_tail->size = 5;


    ck_assert_str_eq(data_head->string, "head");
    ck_assert_str_eq(data_tail->string, "tail");

    int result_head = ds_list_add_head(list, data_head);
    int result_tail = ds_list_add_tail(list, data_tail);

    ck_assert_int_eq(result_head, 0);
    ck_assert_int_eq(result_tail, 0); 

    ck_assert_int_eq(ds_list_size(list),  2);

    /* testing head data went to head node and tail data went to tail node */
    Data *back_head_data = (Data *) ds_list_head(list)->data;
    ck_assert_str_eq(back_head_data->string, "head");
    ck_assert_int_eq(back_head_data->size, 10);
    ck_assert_ptr_eq(data_head, back_head_data);

    Data *back_tail_data = (Data *) ds_list_tail(list)->data;
    ck_assert_str_eq(back_tail_data->string, "tail");
    ck_assert_int_eq(back_tail_data->size, 5);
    ck_assert_ptr_eq(data_tail, back_tail_data);

    /* free and exit */
    ck_assert_int_eq(ds_list_free(&list), ds_ok);
}
END_TEST


START_TEST (test_list_add_next) {
    DS_List_Ptr list;

    ds_list_init(&list, (void *)destroy);

    ck_assert_int_eq(ds_list_size(list),  0);
    Data *data_head, *data_tail, *data_next;

    /* Preparing data_head */
    if ((data_head = (Data *) malloc(sizeof(Data))) == NULL) {
        ck_abort_msg("Couldn't allocate memory for data head, will stop test");
    }

    /* Allocating data_head internal String */ 
    if ((data_head->string = (char *) malloc(6 * sizeof(char))) == NULL)
        ck_abort_msg("Couldn't allocate memory, will stop test");

    memcpy(data_head->string, "head", 5);
    data_head->size = 10;


    /*  Preparing data_tail */
    if ((data_tail = (Data *) malloc(sizeof(Data))) == NULL) {
        ck_abort_msg("Couldn't allocate memory for data tail, will stop test");
    }

    /* Allocating data_head internal String */ 
    if ((data_tail->string = (char *) malloc(6 * sizeof(char))) == NULL)
        ck_abort_msg("Couldn't allocate memory, will stop test");

    memcpy(data_tail->string, "tail", 5);
    data_tail->size = 5;

    /*  Preparing data_next */
    if ((data_next = (Data *) malloc(sizeof(Data))) == NULL) {
        ck_abort_msg("Couldn't allocate memory for data next, will stop test");
    }

    /* Allocating data_head internal String */ 
    if ((data_next->string = (char *) malloc(6 * sizeof(char))) == NULL)
        ck_abort_msg("Couldn't allocate memory, will stop test");

    memcpy(data_next->string, "next", 5);
    data_next->size = 3;


    ck_assert_str_eq(data_head->string, "head");
    ck_assert_str_eq(data_tail->string, "tail");
    ck_assert_str_eq(data_next->string, "next");

    int result_head = ds_list_add_head(list, data_head);
    int result_next = ds_list_add_next(list, list->head, data_next);
    int result_tail = ds_list_add_tail(list, data_tail);

    ck_assert_int_eq(result_head, 0);
    ck_assert_int_eq(result_tail, 0); 
    ck_assert_int_eq(result_next, 0); 

    ck_assert_int_eq(ds_list_size(list),  3);

    /* testing head data went to head node and tail data went to tail node */
    Data *back_head_data = (Data *) ds_list_head(list)->data;
    ck_assert_str_eq(back_head_data->string, "head");
    ck_assert_int_eq(back_head_data->size, 10);
    ck_assert_ptr_eq(data_head, back_head_data);

    Data *back_tail_data = (Data *) ds_list_tail(list)->data;
    ck_assert_str_eq(back_tail_data->string, "tail");
    ck_assert_int_eq(back_tail_data->size, 5);
    ck_assert_ptr_eq(data_tail, back_tail_data);

    Data *back_next_data = (Data *) ds_list_head(list)->next->data;
    ck_assert_str_eq(back_next_data->string, "next");
    ck_assert_int_eq(back_next_data->size, 3);
    ck_assert_ptr_eq(data_next, back_next_data); 

    /* free and exit */
    ck_assert_int_eq(ds_list_free(&list), ds_ok);
}
END_TEST

START_TEST (test_list_rm_next) {
    DS_List_Ptr list;

    ds_list_init(&list, (void *)destroy);

    ck_assert_int_eq(ds_list_size(list),  0);
    Data *data_head, *data_tail, *data_next, *data_tbdl;

    /* Preparing data_head */
    if ((data_head = (Data *) malloc(sizeof(Data))) == NULL) {
        ck_abort_msg("Couldn't allocate memory for data head, will stop test");
    }

    /* Allocating data_head internal String */ 
    if ((data_head->string = (char *) malloc(6 * sizeof(char))) == NULL)
        ck_abort_msg("Couldn't allocate memory, will stop test");

    memcpy(data_head->string, "head", 5);
    data_head->size = 10;


    /*  Preparing data_tail */
    if ((data_tail = (Data *) malloc(sizeof(Data))) == NULL) {
        ck_abort_msg("Couldn't allocate memory for data tail, will stop test");
    }

    /* Allocating data_head internal String */ 
    if ((data_tail->string = (char *) malloc(6 * sizeof(char))) == NULL)
        ck_abort_msg("Couldn't allocate memory, will stop test");

    memcpy(data_tail->string, "tail", 5);
    data_tail->size = 5;

    /*  Preparing data_next */
    if ((data_next = (Data *) malloc(sizeof(Data))) == NULL) {
        ck_abort_msg("Couldn't allocate memory for data next, will stop test");
    }

    /* Allocating data_head internal String */ 
    if ((data_next->string = (char *) malloc(6 * sizeof(char))) == NULL)
        ck_abort_msg("Couldn't allocate memory, will stop test");

    memcpy(data_next->string, "next", 5);
    data_next->size = 3;

    /*  Preparing 'data to be deleted' data_tbdl */
    if ((data_tbdl = (Data *) malloc(sizeof(Data))) == NULL) {
        ck_abort_msg("Couldn't allocate memory for data tbd, will stop test");
    }

    /* Allocating data_head internal String */ 
    if ((data_tbdl->string = (char *) malloc(6 * sizeof(char))) == NULL)
        ck_abort_msg("Couldn't allocate memory, will stop test");

    memcpy(data_tbdl->string, "tbdl", 5);
    data_tbdl->size = 12;


    ck_assert_str_eq(data_head->string, "head");
    ck_assert_str_eq(data_tail->string, "tail");
    ck_assert_str_eq(data_next->string, "next");
    ck_assert_str_eq(data_tbdl->string, "tbdl");

    ck_assert_int_eq(ds_list_add_head(list, data_head), 0);
    ck_assert_int_eq(ds_list_add_next(list, list->head, data_tbdl), 0);
    ck_assert_int_eq(ds_list_add_next(list, list->head->next, data_next), 0);
    ck_assert_int_eq(ds_list_add_tail(list, data_tail), 0);

    ck_assert_int_eq(ds_list_size(list),  4);

    /* testing head data went to head node and tail data went to tail node */
    Data *back_head_data = (Data *) ds_list_head(list)->data;
    ck_assert_str_eq(back_head_data->string, "head");
    ck_assert_int_eq(back_head_data->size, 10);
    ck_assert_ptr_eq(data_head, back_head_data);

    Data *back_tail_data = (Data *) ds_list_tail(list)->data;
    ck_assert_str_eq(back_tail_data->string, "tail");
    ck_assert_int_eq(back_tail_data->size, 5);
    ck_assert_ptr_eq(data_tail, back_tail_data);

    Data *back_next_data = (Data *) ds_list_head(list)->next->next->data;
    ck_assert_str_eq(back_next_data->string, "next");
    ck_assert_int_eq(back_next_data->size, 3);
    ck_assert_ptr_eq(data_next, back_next_data); 
    
    Data *back_tbdl_data = (Data *) ds_list_head(list)->next->data;
    ck_assert_str_eq(back_tbdl_data->string, "tbdl");
    ck_assert_int_eq(back_tbdl_data->size, 12);
    ck_assert_ptr_eq(data_tbdl, back_tbdl_data); 

    Data *deleted_data;
    /* Deleting the tbdl data, which is the node after header */
    ck_assert_int_eq(ds_list_rem_next(list, ds_list_head(list), NULL), ds_error_data_ptr_null);
    ck_assert_int_eq(ds_list_rem_next(list, ds_list_head(list), (void **)&deleted_data), ds_ok);
    /* check the returned data matches the original data */
    ck_assert_mem_eq(deleted_data, back_tbdl_data, sizeof(Data)); 

    /* Check if the data in node after header is now the next_data */
    ck_assert_mem_eq(back_next_data, ds_list_head(list)->next->data, sizeof(Data));

    ck_assert_int_eq(ds_list_size(list),  3);

    /* free and exit */
    ck_assert_int_eq(ds_list_free(&list), ds_ok);
}
END_TEST


Suite * list_suite(void) {
    Suite *s;
    TCase *tc_core;

    s = suite_create("List");

    /* Core test case */
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_list_init);
    tcase_add_test(tc_core, test_list_add_header);
    tcase_add_test(tc_core, test_list_add_tail);
    tcase_add_test(tc_core, test_list_add_next);
    tcase_add_test(tc_core, test_list_rm_next);
    suite_add_tcase(s, tc_core);

    return s;
}

int main(void) {
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = list_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
