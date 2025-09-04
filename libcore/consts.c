/**
 * @file consts.c
 * @brief Defines constant variables used throughout the Core plugin.
 */

#include <stdint.h>
#include "plugin.h"

static const char *fields_arr[] = {
    "Mathematics",
};

static const char *domain_mathematics_arr[] = {

};

static const uint32_t fields_len = 1;
static const uint32_t domain_mathematics_len = 0;

static const Strings domain_mathematics = {
    .strings = domain_mathematics_arr,
    .size = domain_mathematics_len,
};

const Strings fields = {
    .strings = fields_arr,
    .size = fields_len,
};

const Strings *domains[] = {
    &domain_mathematics,
};

const uint32_t domains_len = 1;
