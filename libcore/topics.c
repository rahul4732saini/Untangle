/**
 * @file topics.c
 * @brief Defines structures to store topic-related data.
 *
 * @details This file comprises structures definitions to store the
 * data associated with the domains, fields and problems supported by
 * this plugin library.
 */

#include "structs.h"

const Field fld_basic_algebra = {
    .name = "Elementary Algebra",
    .problems_size = 0,
};

const Field fld_matrix_manipulation = {
    .name = "Matrix Manipulation",
    .problems_size = 0,
};

const Field fld_arithmetic_progression = {
    .name = "Arithmetic Progressions",
    .problems_size = 0,
};

const Field fld_geometric_progression = {
    .name = "Geometric Progressions",
    .problems_size = 0,
};

const Field *fields_math_arr[] = {
    &fld_basic_algebra,
    &fld_matrix_manipulation,
    &fld_arithmetic_progression,
    &fld_geometric_progression,
};

const len_t fields_mat_arr_len = 4;

const Domain dmn_math = {
    .name = "Mathematics",
    .fields = fields_math_arr,
    .fields_size = 4,
};

const Domain *domains_arr[] = {
    &dmn_math,
};

const len_t domains_arr_len = 1;

const Domains domains = {
    .domains = domains_arr,
    .domains_size = domains_arr_len,
};
