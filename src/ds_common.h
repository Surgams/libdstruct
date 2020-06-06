#ifndef DS_COMMON_H
#define DS_COMMON_H

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define ERROR_CODES 20
#define MAX_KEY_SIZE 75

typedef unsigned short ushort;

typedef enum ds_return_code {
    ds_ok = 0,        
    ds_warning_empty,
    ds_warning_last_element,
    ds_warning_element_not_found,
    ds_warning_key_exists,
    ds_warning_member_exists,
    ds_warning_buffer_is_full,
    ds_error_cannot_allocate_memory = ERROR_CODES,
    ds_error_destroy_method,
    ds_error_callback_pointer_null,
    ds_error_in_key_size,
    ds_error_data_ptr_null,
    ds_error_general
} ds_error_code;

#define ERROR_MESSAGE(code) \
    ({ char* code_message; \
        switch(code){ \
            case ds_ok: \
                code_message = "OK, no error"; \
                break; \
            case ds_warning_empty: \
                code_message = "The structure is empty, operation discarded"; \
                break; \
            case ds_warning_last_element: \
                code_message = "Warning: Last element in the structure, next point is null"; \
                break; \
            case ds_warning_element_not_found: \
                code_message = "Warning: Element not found in the structure"; \
                break; \
            case ds_warning_key_exists: \
                code_message = "Warning: The key exists in the hash table"; \
                break; \
            case ds_warning_member_exists: \
                code_message = "Warning: The member exists in the Set"; \
                break; \
            case ds_warning_buffer_is_full: \
                code_message = "Warning: The structure buffer is full"; \
                break; \
            case ds_error_cannot_allocate_memory: \
                code_message = "Error: Failed to allocate memory"; \
                break; \
            case ds_error_destroy_method: \
                code_message = "Error: Failed to destroy element"; \
                break; \
            case ds_error_callback_pointer_null: \
                code_message = "Error: Pointer to callback method is null"; \
                break; \
            case ds_error_in_key_size: \
                code_message = "Error: Provide key size should be more than zero and less than MAX_KEY_SIZE"; \
                break; \
            case ds_error_data_ptr_null: \
                code_message = "Error: The pointer to the the data is null"; \
                break; \
            case ds_error_general: \
                code_message = "Error: Unspecified error"; \
                break; \
            default: \
                code_message = "Undefined Error"; \
     }; \
     code_message; })

#endif
