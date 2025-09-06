/**
 * @file topics.c
 * @brief Defines structures to store topic-related data.
 *
 * @details This file comprises structures definitions to store all the
 * data associated with the domains, fields and problems supported by this
 * plugin library.
 */

#include "structs.h"

Domain dmn_math = {
    .name = "Mathematics",
    .fields_size = 0,
};

Domain *domains_arr[] = {
    &dmn_math,
};

const len_t domains_arr_len = 1;

const Domains domains = {
    .domains = domains_arr,
    .domains_size = domains_arr_len,
};
