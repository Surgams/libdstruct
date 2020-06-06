#include "ds_set.h"

#define ds_set_head ds_list_head 
#define ds_set_tail ds_list_tail 
#define ds_set_data ds_list_data 
#define ds_set_next ds_list_next 
#define ds_set_add_next ds_list_add_next
#define ds_set_rem_next ds_list_rem_next

ushort ds_set_init (DS_Set_Ptr *set, Compare compare, Destroy destroy) {
    if ((*set = (DS_Set_Ptr) malloc(sizeof(DS_Set))) == NULL) 
        return ds_error_cannot_allocate_memory;

    /* Initialise the set here */
    ds_list_init(set, destroy);
    (*set)->compare = compare;
    return ds_ok;
}

ushort ds_set_insert (DS_Set_Ptr set, const void *data) {
    /* Check if duplicate */
    if (ds_set_is_member(set, data))
        return ds_warning_member_exists;

    /* Insert the data */
    return ds_set_add_next(set, ds_set_tail(set), data);
}

ushort ds_set_remove(DS_Set_Ptr set, void **data) {
    DS_Node_Ptr member, prev = NULL;

    if(!data)
        return ds_error_data_ptr_null;

    /* Find the member to remove */
    for (member = ds_set_head(set); member != NULL; member = ds_set_next(member)) {
        if (!set->compare(*data, ds_set_data(member)))
            break;
        prev = member;
    }
    /* if the member is not found */
    if (member == NULL)
        return ds_warning_element_not_found;
    return ds_set_rem_next(set, prev, data);
}

ushort ds_set_union (DS_Set_Ptr *setu, const DS_Set_Ptr set1, const DS_Set_Ptr set2) {
    DS_Node_Ptr member;
    ushort retval;

    /* Initialise the union set */
    ds_set_init(setu, set1->compare, set1->destroy);

    /* Insert the members of the set1 */
    for (member = ds_set_head(set1); member != NULL; member = ds_set_next(member)) {
        if ((retval = ds_set_add_next(*setu, ds_set_tail(*setu), ds_set_data(member))) != ds_ok) {
            ds_set_free(setu);
            return retval;
        }
    }

    /* Insert the members of the set2 */
    for (member = ds_set_head(set2); member != NULL; member = ds_set_next(member)) {
        if (ds_set_is_member(set1, ds_set_data(member))) {
            /* skip duplicates */
            continue;
        } else {
            if ((retval = ds_set_add_next(*setu, ds_set_tail(*setu), ds_set_data(member))) != ds_ok) { 
                ds_set_free(setu);
                return retval;
            }
        }
    }
    return ds_ok;
}

ushort ds_set_intersect (DS_Set_Ptr *seti, const DS_Set_Ptr set1, const DS_Set_Ptr set2) {
    DS_Node_Ptr member;
    ushort retval;

    /* Initialise the intersection set */
    ds_set_init(seti, set1->compare, set1->destroy);

    /* Insert the elements present in both sets */
    for (member = ds_set_head(set1); member != NULL; member = ds_set_next(member)) {
        if (ds_set_is_member(set2, ds_set_data(member))) {
            if ((retval = ds_set_add_next(*seti, ds_set_tail(*seti), ds_set_data(member))) != ds_ok) {
                ds_set_free(seti);
                return retval;
            }
        }
    }
    return ds_ok;
}

ushort ds_set_diff (DS_Set_Ptr *setd, const DS_Set_Ptr set1, const DS_Set_Ptr set2) {
    DS_Node_Ptr member;
    ushort retval;

    /* Initialise the set for the difference */
    ds_set_init(setd, set1->compare, set1->destroy);

    /* Insert the elements of set1 not in set2 */
    for (member = ds_set_head(set1); member != NULL; member = ds_set_next(member)) {
        if (!ds_set_is_member (set2, ds_set_data(member))){
            if ((retval = ds_set_add_next(*setd, ds_set_tail(*setd), ds_set_data(member))) != ds_ok) {
                ds_set_free(setd);
                return retval;
            }
        }
    }
    return ds_ok;
}

bool ds_set_is_member (const DS_Set_Ptr set, const void *data) {
    DS_Node_Ptr member;

    for (member = ds_set_head(set); member != NULL; member = ds_set_next(member)) {
        /* Compare function returns 0 if equal and non zero if not */
        if (!set->compare((void *)data, ds_set_data(member))) {
            return true;
        }
    }
    return false;
}


bool ds_set_is_subset (const DS_Set_Ptr set1, const DS_Set_Ptr set2) {
    DS_Node_Ptr member;

    /* Do a quick test to rule out some cases */
    if (ds_set_size(set1) > ds_set_size(set2)) 
        return false;

    for (member = ds_set_head(set1); member != NULL; member = ds_set_next(member)) {
        if (!ds_set_is_member(set2, ds_set_data(member)))
            return false;
    }
    return true;
}

bool ds_set_is_equal (const DS_Set_Ptr set1, const DS_Set_Ptr set2) {
    /* Do a quick test to rule out some cases */
    if (ds_set_size(set1) != ds_set_size(set2))
        return false;

    /* Sets of the same size are equal if they are subsets */
    return ds_set_is_subset(set1, set2);
}
