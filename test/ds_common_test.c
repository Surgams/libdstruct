/**
 * Copyright (c) 2020 Surgams
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the BSD license. See LICENSE for details.
 **/

#include <string.h>
#include "../src/ds_common.h"
#include <stdlib.h>
#include <check.h>



START_TEST (test_common_codes) {
    ck_assert_int_eq(ds_ok, 0);        
    ck_assert_int_eq(ds_warning_empty, 1);
    ck_assert_int_eq(ds_warning_last_element, 2);
    ck_assert_int_eq(ds_warning_element_not_found, 3);
    ck_assert_int_eq(ds_warning_key_exists, 4);
    ck_assert_int_eq(ds_warning_member_exists, 5);
    ck_assert_int_eq(ds_error_cannot_allocate_memory, 20);
    ck_assert_int_eq(ds_error_destroy_method, 21);
    ck_assert_int_eq(ds_error_callback_pointer_null, 22);
    ck_assert_int_eq(ds_error_general, 25);
}
END_TEST

START_TEST (test_common_messages) {
    ck_assert_str_eq(ERROR_MESSAGE(ds_ok), "OK, no error");        
    ck_assert_str_eq(ERROR_MESSAGE(ds_warning_empty), "The structure is empty, operation discarded");
    ck_assert_str_eq(ERROR_MESSAGE(ds_warning_last_element), "Warning: Last element in the structure, next point is null");
    ck_assert_str_eq(ERROR_MESSAGE(ds_warning_element_not_found), "Warning: Element not found in the structure");
    ck_assert_str_eq(ERROR_MESSAGE(ds_warning_key_exists), "Warning: The key exists in the hash table");
    ck_assert_str_eq(ERROR_MESSAGE(ds_warning_member_exists), "Warning: The member exists in the Set");
    ck_assert_str_eq(ERROR_MESSAGE(ds_error_cannot_allocate_memory), "Error: Failed to allocate memory");
    ck_assert_str_eq(ERROR_MESSAGE(ds_error_destroy_method), "Error: Failed to destroy element");
    ck_assert_str_eq(ERROR_MESSAGE(ds_error_callback_pointer_null), "Error: Pointer to callback method is null");
    ck_assert_str_eq(ERROR_MESSAGE(ds_error_in_key_size), "Error: Provide key size should be more than zero and less than MAX_KEY_SIZE");
    ck_assert_str_eq(ERROR_MESSAGE(ds_error_data_ptr_null), "Error: The pointer to the the data is null");
    ck_assert_str_eq(ERROR_MESSAGE(ds_error_general), "Error: Unspecified error");
    ck_assert_str_eq(ERROR_MESSAGE(100), "Undefined Error");
}
END_TEST


Suite * queue_suite(void) {
    Suite *s;
    TCase *tc_core;

    s = suite_create("DS Common");

    /* Core test case */
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_common_codes);
    tcase_add_test(tc_core, test_common_messages);
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
