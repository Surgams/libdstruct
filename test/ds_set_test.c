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
#include <check.h>
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

/* Start Unit tests */

START_TEST (test_set_init) {
    DS_Set_Ptr set;

    ds_set_init(&set, (void *)compare, (void *)destroy);

    ck_assert_int_eq(ds_set_size(set),  0);
    ck_assert_int_eq(ds_set_free(&set), ds_ok);
}
END_TEST

START_TEST (test_set_insert_remove) {
    DS_Set_Ptr set;

    ds_set_init(&set, (void *)compare, (void *)destroy);

    ck_assert_int_eq(ds_set_size(set),  0);
    Data *data, *data2, *temp_data, *data3;

    /*Initialise data */
    if ((data = (Data *) malloc(sizeof(Data))) == NULL) 
        ck_abort_msg("Couldn't allocate memory, will stop test");

    data->size = 10;

    if ((data->string = (char *) malloc((data->size + 1) * sizeof(char))) == NULL)
        ck_abort_msg("Couldn't allocate memory, will stop test");

    snprintf(data->string, 7, "%s", "test");
    ck_assert_str_eq(data->string, "test");

    /* Insert and check size */
    ck_assert_int_eq(ds_set_insert(set, data), ds_ok); 
    ck_assert_int_eq(ds_set_size(set),  1);

    ck_assert_msg(ds_set_is_member(set, data), "data is a member of the set");

    /* Initialise data2 */
    if ((data2 = (Data *) malloc(sizeof(Data))) == NULL) 
        ck_abort_msg("Couldn't allocate memory, will stop test");

    data2->size = 12;
    if ((data2->string = (char *) malloc((data2->size)  * sizeof(char))) == NULL)
        ck_abort_msg("Couldn't allocate memory, will stop test");

    snprintf(data2->string, 7, "%s", "test2");
    ck_assert_str_eq(data2->string, "test2");

    ck_assert_msg(!ds_set_is_member(set, data2), "Data2 is not member of set");

    /* Insert data2 and check again */
    ck_assert_int_eq(ds_set_insert(set, data2), ds_ok); 
    ck_assert_msg(ds_set_is_member(set, data2), "Data2 is member of set");

    /* Remove data2 and check again */
    temp_data = data2;

    ck_assert_int_eq(ds_set_remove(set, (void **)&temp_data), ds_ok);
    ck_assert_ptr_eq(data2, temp_data);

    // making sure data2 is intact
    ck_assert_str_eq(data2->string, "test2");


    ck_assert_int_eq(ds_set_size(set),  1);
    ck_assert_msg(!ds_set_is_member(set, data2), "data is a member of the set");

    /* Insert a duplicate and check size */
    ck_assert_int_eq(ds_set_insert(set, data), ds_warning_member_exists); 
    ck_assert_int_eq(ds_set_size(set),  1);

    /* Remove NULL value */
    ck_assert_int_eq(ds_set_remove(set, NULL), ds_error_data_ptr_null); 

    /* Initialise data2 */
    if ((data3 = (Data *) malloc(sizeof(Data))) == NULL) 
        ck_abort_msg("Couldn't allocate memory, will stop test");

    data3->size = 20;
    if ((data3->string = (char *) malloc((data3->size)  * sizeof(char))) == NULL)
        ck_abort_msg("Couldn't allocate memory, will stop test");

    snprintf(data2->string, 10, "%s", "new elem");

    /* Remove no existent element */
    ck_assert_int_eq(ds_set_remove(set, (void**) &data3), ds_warning_element_not_found); 

    free(data2->string);
    free(data2);
    free(data3->string);
    free(data3);
    ck_assert_int_eq(ds_set_free(&set), ds_ok);
}
END_TEST



START_TEST (test_set_union_int) {
    DS_Set_Ptr set1, set2, setu = NULL, seti = NULL;

    ds_set_init(&set1, (void *)compare, (void *)destroy);
    ds_set_init(&set2, (void *)compare, (void *)destroy);

    Data *data, *data2;

    /*Initialise data*/
    if ((data = (Data *) malloc(sizeof(Data))) == NULL) 
        ck_abort_msg("Couldn't allocate memory, will stop test");

    data->size = 10;

    if ((data->string = (char *) malloc((data->size + 1) * sizeof(char))) == NULL)
        ck_abort_msg("Couldn't allocate memory, will stop test");

    snprintf(data->string, 7, "%s", "test");

    /* Insert into data into s1 */
    ck_assert_int_eq(ds_set_insert(set1, data), ds_ok); 
    ck_assert_int_eq(ds_set_size(set1),  1);

    /* Initialise data2 */
    if ((data2 = (Data *) malloc(sizeof(Data))) == NULL) 
        ck_abort_msg("Couldn't allocate memory, will stop test");

    data2->size = 12;
    if ((data2->string = (char *) malloc((data2->size)  * sizeof(char))) == NULL)
        ck_abort_msg("Couldn't allocate memory, will stop test");

    snprintf(data2->string, 7, "%s", "test2");

    /* Insert data2 into s2 */
    ck_assert_int_eq(ds_set_insert(set2, data2), ds_ok); 
    ck_assert_int_eq(ds_set_size(set2),  1);

    /* Test Union */
    ck_assert_int_eq(ds_set_union(&setu, set1, set2), ds_ok);
    ck_assert_int_eq(ds_set_size(setu), 2);

    /* Test Intersection */
    ck_assert_int_eq(ds_set_intersect(&seti, set1, set2), ds_ok);
    /* Should be be empty */
    ck_assert_int_eq(ds_set_size(seti), 0);

    /* Insert data into s2 */
    ck_assert_int_eq(ds_set_insert(set2, data), ds_ok); 
    ck_assert_int_eq(ds_set_size(set2),  2);

    free(seti);
    seti = NULL;

    ck_assert_int_eq(ds_set_intersect(&seti, set1, set2), ds_ok);
    ck_assert_int_eq(ds_set_size(seti), 1);
    
    free(data->string);
    free(data);
    free(data2->string);
    free(data2);
    ck_assert_int_eq(ds_set_free(&setu), ds_ok);
    free(set2);
    ck_assert_int_eq(ds_set_free(&set1), ds_ok);
}
END_TEST

START_TEST (test_set_diff) {
    DS_Set_Ptr set1, set2, setd = NULL;

    ds_set_init(&set1, (void *)compare, (void *)destroy);
    ds_set_init(&set2, (void *)compare, (void *)destroy);

    Data *data, *data2;

    /*Initialise data*/
    if ((data = (Data *) malloc(sizeof(Data))) == NULL) 
        ck_abort_msg("Couldn't allocate memory, will stop test");

    data->size = 10;

    if ((data->string = (char *) malloc((data->size + 1) * sizeof(char))) == NULL)
        ck_abort_msg("Couldn't allocate memory, will stop test");

    snprintf(data->string, 7, "%s", "test");

    /* Insert into data into s1 */
    ck_assert_int_eq(ds_set_insert(set1, data), ds_ok); 
    ck_assert_int_eq(ds_set_size(set1),  1);

    /* Initialise data2 */
    if ((data2 = (Data *) malloc(sizeof(Data))) == NULL) 
        ck_abort_msg("Couldn't allocate memory, will stop test");

    data2->size = 12;
    if ((data2->string = (char *) malloc((data2->size)  * sizeof(char))) == NULL)
        ck_abort_msg("Couldn't allocate memory, will stop test");

    snprintf(data2->string, 7, "%s", "test2");

    /* Insert data and data2 into s2 */
    ck_assert_int_eq(ds_set_insert(set2, data2), ds_ok); 
    ck_assert_int_eq(ds_set_size(set2),  1);
    ck_assert_int_eq(ds_set_insert(set2, data), ds_ok); 
    ck_assert_int_eq(ds_set_size(set2),  2);


    /* Test Diff */
    ck_assert_int_eq(ds_set_intersect(&setd, set2, set1), ds_ok);
    ck_assert_int_eq(ds_set_size(setd), 1);
    
    free(data->string);
    free(data);
    free(data2->string);
    free(data2);
    free(setd);
    free(set2);
    ck_assert_int_eq(ds_set_free(&set1), ds_ok);
}
END_TEST

START_TEST (test_set_subset_equal_member) {
    DS_Set_Ptr set1, set2;

    ds_set_init(&set1, (void *)compare, (void *)destroy);
    ds_set_init(&set2, (void *)compare, (void *)destroy);

    Data *data, *data2;

    /*Initialise data*/
    if ((data = (Data *) malloc(sizeof(Data))) == NULL) 
        ck_abort_msg("Couldn't allocate memory, will stop test");

    data->size = 10;

    if ((data->string = (char *) malloc((data->size + 1) * sizeof(char))) == NULL)
        ck_abort_msg("Couldn't allocate memory, will stop test");

    snprintf(data->string, 7, "%s", "test");

    /* Initialise data2 */
    if ((data2 = (Data *) malloc(sizeof(Data))) == NULL) 
        ck_abort_msg("Couldn't allocate memory, will stop test");

    data2->size = 12;
    if ((data2->string = (char *) malloc((data2->size)  * sizeof(char))) == NULL)
        ck_abort_msg("Couldn't allocate memory, will stop test");

    snprintf(data2->string, 7, "%s", "test2");
    
    /* Insert into data into s1 */
    ck_assert_int_eq(ds_set_insert(set1, data), ds_ok); 
    ck_assert_int_eq(ds_set_size(set1),  1);

    ck_assert(ds_set_is_member(set1, data));
    ck_assert(!ds_set_is_member(set1, data2));

    /* Insert data and data2 into s2 */
    ck_assert(!ds_set_is_member(set2, data));
    ck_assert(!ds_set_is_member(set2, data2));
    ck_assert(!ds_set_is_equal(set2, set1));
    
    ck_assert_int_eq(ds_set_insert(set2, data), ds_ok); 
    ck_assert_int_eq(ds_set_size(set2),  1);

    ck_assert(ds_set_is_member(set2, data));
    ck_assert(ds_set_is_equal(set2, set1));

    ck_assert_int_eq(ds_set_insert(set2, data2), ds_ok); 
    ck_assert_int_eq(ds_set_size(set2),  2);
  
    ck_assert(ds_set_is_member(set2, data2));
    ck_assert(!ds_set_is_equal(set2, set1));
    
    ck_assert(!ds_set_is_subset(set2, set1));
    ck_assert(ds_set_is_subset(set1, set2));

    free(data->string);
    free(data);
    free(data2->string);
    free(data2);
    free(set2);
    ck_assert_int_eq(ds_set_free(&set1), ds_ok);
}
END_TEST


Suite * set_suite(void) {
    Suite *s;
    TCase *tc_core;

    s = suite_create("List");

    /* Core test case */
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_set_init);
    tcase_add_test(tc_core, test_set_insert_remove);
    tcase_add_test(tc_core, test_set_union_int);
    tcase_add_test(tc_core, test_set_diff);
    tcase_add_test(tc_core, test_set_subset_equal_member);
    suite_add_tcase(s, tc_core);

    return s;
}

int main(void) {
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = set_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
