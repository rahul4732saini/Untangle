/**
 * @file registry.c
 * @brief Defines and exports the data associated with this plugin.
 */

#if defined(_WIN32) || defined(_WIN64)
#define EXPORT __declspec(dllexport)

#else
#define EXPORT __attribute__((visibility("default")))

#endif

#include "structs.h"

// Name of the plugin to be displayed in the application interface.
EXPORT const char *plugin_name = "Core Plugin";

const Field fld_basic_algebra = {
    .name = "Elementary Algebra",
    .size = 0,
};

const Field fld_matrix_manipulation = {
    .name = "Matrix Manipulation",
    .size = 0,
};

const Field fld_arithmetic_progression = {
    .name = "Arithmetic Progressions",
    .size = 0,
};

const Field fld_geometric_progression = {
    .name = "Geometric Progressions",
    .size = 0,
};

const Field *fields_math_arr[] = {
    &fld_basic_algebra,
    &fld_matrix_manipulation,
    &fld_arithmetic_progression,
    &fld_geometric_progression,
};

const len_t fields_math_arr_len = 4;

const Domain dmn_math = {
    .name = "Mathematics",
    .fields = fields_math_arr,
    .size = fields_math_arr_len,
};

const Domain *domains_arr[] = {
    &dmn_math,
};

const len_t domains_arr_len = 1;

// Hierarchy of domains, fields, and problems for users to choose.
EXPORT const Domains domains = {
    .domains = domains_arr,
    .size = domains_arr_len,
};
