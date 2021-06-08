/**
 * Copyright (c) 2020 Surgams
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the BSD license. See LICENSE for details.
 **/

#include <stdio.h>
#include <string.h>
#include "../src/ds_common.h"
#include "../src/ds_hash.c"
#include <stdlib.h>
#include <check.h>


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

/* Start Unit tests */


/* Testing first the static methods and internal macros*/

START_TEST (test_comp_keys) {
    const char *some_key = "Key The works";
    const char *different_key = "Different Key";
    DS_Hash_Node_Ptr node;
    node = (DS_Hash_Node_Ptr) malloc(sizeof(DS_Hash_Node));
    node->key = (char *) malloc(sizeof(char) * MAX_KEY_SIZE);
    memcpy(node->key, some_key, 8);
    ck_assert_int_eq(DS_CHK_KEYS_MATCH(node, some_key), true);
    ck_assert_int_eq(DS_CHK_KEYS_MATCH(node, different_key), false);
    free(node->key);
    free(node);
}
END_TEST

START_TEST (test_is_prime) {
    ck_assert_uint_eq(ds_isprime(1103), true);
    ck_assert_uint_eq(ds_isprime(1988), false);
    ck_assert_uint_eq(ds_isprime(17389), true);
    ck_assert_uint_eq(ds_isprime(17390), false);
}
END_TEST

START_TEST (test_roundup_to_prime) {
    ck_assert_uint_eq(ds_isprime(ds_roundup_to_prime(1103)), true);
    ck_assert_uint_eq(ds_isprime(ds_roundup_to_prime(1988)), true);
    ck_assert_uint_eq(ds_isprime(ds_roundup_to_prime(17388)), true);
    ck_assert_uint_eq(ds_isprime(ds_roundup_to_prime(17390)), true);
    ck_assert_uint_eq(ds_isprime(ds_roundup_to_prime(1123)), true);
}
END_TEST

START_TEST (test_create_hash_node) {
    const char *some_key = "Key The works";

    /* Create new data type */
    Data_Ptr data = (Data_Ptr) malloc(sizeof(Data));
    data->size = 10; /* any number */
    data->string = (char *) malloc(6 * sizeof(char));
    memcpy(data->string, "first", 6);
    data->string[5] = '\0';

    /*Test if data been created correctly */
    ck_assert_int_eq(data->size, 10);
    ck_assert_str_eq(data->string, "first");


    unsigned short error_code = 0;
    DS_Hash_Node_Ptr node = ds_create_hash_node (some_key, data, &error_code);
    ck_assert_int_eq(error_code, ds_ok);
    ck_assert_str_eq(node->key, some_key);
    ck_assert_int_eq(DS_CHK_KEYS_MATCH(node, some_key), true);
    ck_assert_ptr_eq(node->data, data);
    ck_assert_str_eq(((Data_Ptr)(node->data))->string, "first");
    destroy(data);
    free(node->key);
    free(node);
}
END_TEST

START_TEST (test_create_hash_node_wrong_keysize) {
    /* Create new data type */
    Data_Ptr data = (Data_Ptr) malloc(sizeof(Data));
    data->size = 10; /* any number */
    data->string = (char *) malloc(6 * sizeof(char));
    memcpy(data->string, "first", 6);
    data->string[5] = '\0';

    /*Test if data been created correctly */
    ck_assert_int_eq(data->size, 10);
    ck_assert_str_eq(data->string, "first");

    char some_key[75];
    memset(some_key, 'x', 75);

    unsigned short error_code = 0;
    DS_Hash_Node_Ptr node = ds_create_hash_node (some_key, data, &error_code);
    ck_assert_int_eq(error_code, ds_error_in_key_size);
    ck_assert_ptr_eq(node, NULL);

    char some_other_key[76];
    memset(some_other_key, 'x', 76);
    some_other_key[75] = '\0';

    error_code = 0;
    node = ds_create_hash_node (some_other_key, data, &error_code);
    ck_assert_int_eq(error_code, ds_error_in_key_size);
    ck_assert_ptr_eq(node, NULL);

    destroy(data);
}
END_TEST

START_TEST (test_hash_table_node_destroy) {
    /* Create new data type */
    Data_Ptr data = (Data_Ptr) malloc(sizeof(Data));
    data->size = 10; /* any number */
    data->string = (char *) malloc(6 * sizeof(char));
    memcpy(data->string, "first", 6);
    data->string[5] = '\0';

    /*Test if data been created correctly */
    ck_assert_int_eq(data->size, 10);
    ck_assert_str_eq(data->string, "first");

    char some_key[75] = {'\0'};
    memcpy(some_key, "key", 75);

    /* Create hash table */ 
    DS_Hash_Table_Ptr htbl;
    int bucket_number = 2;
    ck_assert_int_eq (ds_hash_init (&htbl, bucket_number, NULL, (void *)destroy), ds_ok);
    ck_assert_ptr_nonnull(htbl);
    
    /* verify the buckets been created and initialized */
    for (unsigned i = 0; i < ds_roundup_to_prime(bucket_number); i++) {
        ck_assert_ptr_nonnull(&(htbl->table[i]));
        ck_assert_int_eq(htbl->table[i].size, 0);
    }

    unsigned short error_code = 0;
    DS_Hash_Node_Ptr node = ds_create_hash_node (some_key, data, &error_code);
    ck_assert_int_eq(error_code, ds_ok);
    ck_assert_str_eq(node->key, some_key);
    ck_assert_int_eq(DS_CHK_KEYS_MATCH(node, some_key), true);
    ck_assert_ptr_eq(node->data, data);
    ck_assert_str_eq(((Data_Ptr)(node->data))->string, "first");

    ck_assert_int_eq(ds_hash_node_destroy(htbl, node, true), ds_ok);
    ck_assert_ptr_null(data->string);
    ck_assert_int_eq (ds_hash_free (&htbl), ds_ok);
}
END_TEST

START_TEST (test_hash_table_destroy) {
    /* Create new data type */
    Data_Ptr data = (Data_Ptr) malloc(sizeof(Data));
    data->size = 10; /* any number */
    data->string = (char *) malloc(6 * sizeof(char));
    memcpy(data->string, "first", 6);
    data->string[5] = '\0';

    /*Test if data been created correctly */
    ck_assert_int_eq(data->size, 10);
    ck_assert_str_eq(data->string, "first");

    char some_key[75] = {'\0'};
    memcpy(some_key, "key", 75);

    /* Create hash table */ 
    DS_Hash_Table_Ptr htbl;
    int bucket_number = 2;
    ck_assert_int_eq (ds_hash_init (&htbl, bucket_number, NULL, (void *)destroy), ds_ok);
    ck_assert_ptr_nonnull(htbl);
   
    /* verify the buckets been created and initialized */
    for (unsigned i = 0; i < ds_roundup_to_prime(bucket_number); i++) {
        ck_assert_ptr_nonnull(&(htbl->table[i]));
        ck_assert_int_eq(htbl->table[i].size, 0);
    }

    ck_assert_int_eq(ds_hash_put(htbl, some_key, data), ds_ok); 
    ck_assert_int_eq(htbl->size, 1);
   
    /* Check if the data exists now */
    Data_Ptr data_back;
    ck_assert_int_eq(ds_hash_get(htbl, some_key, (void **)&data_back), ds_ok);
    ck_assert_str_eq(data_back->string, data->string);

    ck_assert_int_eq(ds_hash_destroy(htbl), ds_ok);
    ck_assert_int_eq(ds_hash_size(htbl), 0);
    ck_assert_int_eq(htbl->buckets, 0);

    /* Should not get back anything */
    ck_assert_int_ne(ds_hash_get(htbl, some_key, (void **)&data_back), ds_ok);
    ck_assert_ptr_null(data->string);
    ck_assert_int_eq(ds_hash_free (&htbl), ds_ok);
}
END_TEST

/* End of static methods */

START_TEST (test_hash_table_init) {
    DS_Hash_Table_Ptr htbl;
    int bucket_number = 13;
    ck_assert_int_eq (ds_hash_init (&htbl, bucket_number, NULL, (void *)destroy), ds_ok);
    ck_assert_ptr_nonnull(htbl);
     
    /* verify the buckets been created and initialized */
    for (unsigned i = 0; i < ds_roundup_to_prime(bucket_number); i++) {
        ck_assert_ptr_nonnull(&(htbl->table[i]));
        ck_assert_int_eq(htbl->table[i].size, 0);
    }

    ck_assert_int_eq (ds_hash_free (&htbl), ds_ok);
}
END_TEST

START_TEST (test_hash_table_remove) {
    /* Create new data type */
    Data_Ptr data = (Data_Ptr) malloc(sizeof(Data));
    data->size = 10; /* any number */
    data->string = (char *) malloc(6 * sizeof(char));
    memcpy(data->string, "first", 6);
    data->string[5] = '\0';

    /*Test if data been created correctly */
    ck_assert_int_eq(data->size, 10);
    ck_assert_str_eq(data->string, "first");

    char some_key[75] = {'\0'};
    memcpy(some_key, "key", 75);

    /* Create hash table */ 
    DS_Hash_Table_Ptr htbl;
    int bucket_number = 2;
    ck_assert_int_eq (ds_hash_init (&htbl, bucket_number, NULL, (void *)destroy), ds_ok);
    ck_assert_ptr_nonnull(htbl);
   
    /* verify the buckets been created and initialized */
    for (unsigned i = 0; i < ds_roundup_to_prime(bucket_number); i++) {
        ck_assert_ptr_nonnull(&(htbl->table[i]));
        ck_assert_int_eq(htbl->table[i].size, 0);
    }

    /* Put element to the table */
    ck_assert_int_eq(ds_hash_put(htbl, some_key, data), ds_ok); 
    ck_assert_int_eq(htbl->size, 1);
   
    /* Check if the data exists now */
    Data_Ptr data_back;
    ck_assert_int_eq(ds_hash_get(htbl, some_key, (void **)&data_back), ds_ok);
    ck_assert_str_eq(data_back->string, data->string);
    
    /* Remove that node */
    ck_assert_int_eq(ds_hash_remove (htbl, some_key, (void **)&data_back), ds_ok);
    ck_assert_str_eq(data_back->string, data->string);
    /* we shouldn't be able to get again */
    ck_assert_int_ne(ds_hash_get(htbl, some_key, (void **)&data_back), ds_ok);
    ck_assert_int_eq(ds_hash_get(htbl, some_key, (void **)&data_back), ds_warning_empty);
    ck_assert_int_eq(htbl->size, 0);

    /* Remove element that does exist */
    /* Put an element to the table */
    ck_assert_int_eq(ds_hash_put(htbl, some_key, data), ds_ok); 
    ck_assert_int_eq(htbl->size, 1);

    ck_assert_int_eq(ds_hash_remove (htbl, "some_key_", (void **)&data_back), ds_warning_element_not_found);
    ck_assert_int_eq(htbl->size, 1);
    ck_assert_int_eq(ds_hash_free (&htbl), ds_ok);
}
END_TEST

START_TEST (test_hash_table_put_get) {
    /* Create new data */
    Data_Ptr data1 = (Data_Ptr) malloc(sizeof(Data));
    data1->size = 10; /* any number */
    data1->string = (char *) malloc(6 * sizeof(char));
    memcpy(data1->string, "first", 6);
    data1->string[5] = '\0';

    /*Test if data been created correctly */
    ck_assert_int_eq(data1->size, 10);
    ck_assert_str_eq(data1->string, "first");

    char some_key1[75] = {'\0'};
    memcpy(some_key1, "key1", 75);

    Data_Ptr data2 = (Data_Ptr) malloc(sizeof(Data));
    data2->size = 12; /* any number */
    data2->string = (char *) malloc(7 * sizeof(char));
    memcpy(data2->string, "second", 7);
    data2->string[6] = '\0';

    /*Test if data been created correctly */
    ck_assert_int_eq(data2->size, 12);
    ck_assert_str_eq(data2->string, "second");

    char some_key2[75] = {'\0'};
    memcpy(some_key2, "key2", 75);

    /* Create hash table */ 
    DS_Hash_Table_Ptr htbl;
    int bucket_number = 22;
    ck_assert_int_eq (ds_hash_init (&htbl, bucket_number, NULL, (void *)destroy), ds_ok);
    ck_assert_ptr_nonnull(htbl);
   
    /* verify the buckets been created and initialized */
    for (unsigned i = 0; i < ds_roundup_to_prime(bucket_number); i++) {
        ck_assert_ptr_nonnull(&(htbl->table[i]));
        ck_assert_int_eq(htbl->table[i].size, 0);
    }

    /* Put element to the table */
    ck_assert_int_eq(ds_hash_put(htbl, some_key1, data1), ds_ok); 
    ck_assert_int_eq(htbl->size, 1);
    ck_assert_int_eq(ds_hash_put(htbl, some_key2, data2), ds_ok); 
    ck_assert_int_eq(htbl->size, 2);
   
    /* Check if the data exists now */
    Data_Ptr data_back1;
    ck_assert_int_eq(ds_hash_get(htbl, some_key1, NULL), ds_error_data_ptr_null);
    ck_assert_int_eq(ds_hash_get(htbl, some_key1, (void **)&data_back1), ds_ok);
    ck_assert_str_eq(data_back1->string, data1->string);
    
    Data_Ptr data_back2;
    ck_assert_int_eq(ds_hash_get(htbl, some_key2, NULL), ds_error_data_ptr_null);
    ck_assert_int_eq(ds_hash_get(htbl, some_key2, (void **)&data_back2), ds_ok);
    ck_assert_str_eq(data_back2->string, data2->string);
    
    /* Remove the first node */
    ck_assert_int_eq(ds_hash_remove(htbl, some_key1, NULL), ds_error_data_ptr_null);
    ck_assert_int_eq(ds_hash_remove(htbl, some_key1, (void **)&data_back1), ds_ok);
    ck_assert_str_eq(data_back1->string, data1->string);
    ck_assert_int_eq(htbl->size, 1);
    
    /* we shouldn't be able to get again */
    ck_assert_int_ne(ds_hash_get(htbl, some_key1, (void **)&data_back1), ds_ok);
    ck_assert_int_eq(ds_hash_get(htbl, some_key1, (void **)&data_back1), ds_warning_element_not_found);
    ck_assert_int_eq(htbl->size, 1);

    /* Remove the first node */
    ck_assert_int_eq(ds_hash_remove (htbl, some_key2, (void **)&data_back2), ds_ok);
    ck_assert_str_eq(data_back2->string, data2->string);
    ck_assert_int_eq(htbl->size, 0);
    
    ck_assert_int_eq(ds_hash_get(htbl, some_key1, (void **)&data_back1), ds_warning_empty);
    ck_assert_int_eq(ds_hash_get(htbl, some_key2, (void **)&data_back2), ds_warning_empty);

    ck_assert_int_eq(ds_hash_free (&htbl), ds_ok);
}
END_TEST


Suite * list_suite(void) {
    Suite *s;
    TCase *tc_core;

    s = suite_create("Hash");

    /* Core test case */
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_comp_keys);
    tcase_add_test(tc_core, test_is_prime);
    tcase_add_test(tc_core, test_roundup_to_prime);
    tcase_add_test(tc_core, test_create_hash_node);
    tcase_add_test(tc_core, test_create_hash_node_wrong_keysize);
    tcase_add_test(tc_core, test_hash_table_node_destroy);
    tcase_add_test(tc_core, test_hash_table_destroy);
    tcase_add_test(tc_core, test_hash_table_init);
    tcase_add_test(tc_core, test_hash_table_remove);
    tcase_add_test(tc_core, test_hash_table_put_get);

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
