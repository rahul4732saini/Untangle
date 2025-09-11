/**
 * @brief Defines functions for handling session-related data.
 */

#include <stdlib.h>
#include <string.h>

#include "typedefs.h"
#include "consts.h"
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

/**
 * @brief Frees the memory allocated for the domains stored within
 * the SessionData struct.
 */
void clean_domain_tree(void)
{
    for (len_t i = 0; i < sdata.domains->size; ++i)
    {
        clean_field_tree(sdata.domains->domains[i]);
        free(sdata.domains->domains[i]);
    }
}

/**
 * @brief Merges problems into a single Field struct and drops an duplicate
 * problems found within the fields.
 */
void merge_problems(Field *dest, Field *src)
{
    HashTable *table = init_hash_table(default_hash_table_size, get_domain_name);

    // Stores the destination problems within the hash table.
    for (index_t i = 0; i < dest->size; ++i)
        add_table_data(table, dest->problems[i]);

    // Merges problems present in the source struct into the destination struct.
    for (index_t i = 0; i < src->size; ++i)
    {
        Problem *problem = get_table_data(table, src->problems[i]->name);

        // Skips the problem if another one with the same name is
        // already present in this field.
        if (problem)
            continue;

        // Allocates space for the problem and copies its data.
        dest->problems = (Problem **)realloc(dest->problems, (dest->size + 1) * sizeof(Problem *));
        dest->problems[dest->size] = (Problem *)calloc(1, sizeof(Problem));
        *dest->problems[dest->size] = *src->problems[i];

        ++dest->size;
    }
}
