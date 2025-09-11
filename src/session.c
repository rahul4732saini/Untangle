/**
 * @brief Defines functions for handling session-related data.
 */

#include <stdlib.h>
#include <string.h>

#include "typedefs.h"
#include "arch.h"
#include "hash.h"
#include "shared.h"

static SessionData sdata;

/**
 * @brief Extracts the domain name from the specified Domain struct.
 */
static char *get_domain_name(void *data)
{
    return ((Domain *)data)->name;
}

/**
 * @brief Extracts the field name from the specified Field struct.
 */
static char *get_field_name(void *data)
{
    return ((Field *)data)->name;
}

/**
 * @brief Extracts the problem name from the specified Problem struct.
 */
static char *get_problem_name(void *data)
{
    return ((Problem *)data)->name;
}

/**
 * @brief Fress the memory allocated for storing fields
 * and their associated problems.
 *
 * @param domain Pointer to the domain struct comprising
 * domain-related data.
 */
void clean_field_tree(Domain *domain)
{
    for (len_t i = 0; i < domain->size; ++i)
    {
        Field *field = domain->fields[i];

        for (len_t i = 0; i < field->size; ++i)
            free(field->problems[i]);

        free(field);
    }
}
