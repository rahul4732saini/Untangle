/**
 * @file session.c
 * @brief Defines functions for initializing and managing session-related data.
 */

#include <stdlib.h>
#include <string.h>

#include "typedefs.h"
#include "consts.h"
#include "platform.h"
#include "hash.h"
#include "shared.h"

static SessionData sdata;

/**
 * @brief Extracts the domain name from the specified Domain struct.
 * @param data Pointer to the Domain struct.
 */
static char *get_domain_name(void *data)
{
    return ((Domain *)data)->name;
}

/**
 * @brief Extracts the field name from the specified Field struct.
 * @param data Pointer to the Field struct.
 */
static char *get_field_name(void *data)
{
    return ((Field *)data)->name;
}

/**
 * @brief Extracts the problem name from the specified Problem struct.
 * @param data Pointer to the Problem struct.
 */
static char *get_problem_name(void *data)
{
    return ((Problem *)data)->name;
}

/**
 * @brief Fress the heap memory allocated for storing the fields
 * and their associated problems.
 *
 * @param domain Pointer to the domain struct comprising
 * domain-related data.
 */
static void clean_field_tree(Domain *domain)
{
    for (index_t i = 0; i < domain->size; ++i)
    {
        Field *field = domain->fields[i];

        for (index_t j = 0; j < field->size; ++j)
            free(field->problems[j]);

        free(field);
    }
}

/**
 * @brief Frees the memory allocated for the domains stored within
 * the SessionData struct.
 */
static void clean_domain_tree(void)
{
    for (len_t i = 0; i < sdata.domains->size; ++i)
    {
        Domain *domain = sdata.domains->domains[i];

        clean_field_tree(domain);
        free(domain);
    }

    // Frees the memory allocated for the domains array.
    free(sdata.domains->domains);

    // Resets the domains struct members to their defaults.
    sdata.domains->domains = NULL;
    sdata.domains->size = 0;
}

/**
 * @brief Merges problems into a single Field struct and drops an duplicate
 * problems found within the fields.
 */
static void merge_problems(Field *dest, Field *src)
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

/**
 * @brief Merges fields into a single Domain struct while also handling
 * conflicts.
 */
static void merge_fields(Domain *dest, Domain *src)
{
    HashTable *table = init_hash_table(default_hash_table_size, get_domain_name);

    // Stores the destinations fields in the hash table.
    for (index_t i = 0; i < dest->size; ++i)
        add_table_data(table, dest->fields[i]);

    // Merges fields present in the source struct into the destination struct.
    for (index_t i = 0; i < src->size; ++i)
    {
        Field *field = get_table_data(table, src->fields[i]->name);

        // Allocates space for a new field if not already existing in the
        // destination structure.
        if (!field)
        {
            dest->fields = (Field **)realloc(dest->fields, (dest->size + 1) * sizeof(Field *));
            dest->fields[dest->size] = (Field *)calloc(1, sizeof(Field));
            dest->fields[dest->size]->name = src->fields[i]->name;

            field = dest->fields[dest->size];
            ++dest->size;
        }

        // Merges the problems present within the structures.
        merge_problems(field, src->fields[i]);
    }
}

/**
 * @brief Merges the domains into a single Domains struct.
 */
static void merge_domains(Domains *dest, Domains *src)
{
    HashTable *table = init_hash_table(30, get_domain_name);

    // Stores the destinations domains in the hash table.
    for (index_t i = 0; i < dest->size; ++i)
        add_table_data(table, dest->domains[i]);

    for (index_t i = 0; i < src->size; ++i)
    {
        Domain *domain = get_table_data(table, src->domains[i]->name);

        // Allocates space for a new domain if not already existing in the
        // destination structure.
        if (!domain)
        {
            dest->domains = (Domain **)realloc(dest->domains, (dest->size + 1) * sizeof(Domain *));
            dest->domains[dest->size] = (Domain *)calloc(1, sizeof(Domain));
            dest->domains[dest->size]->name = src->domains[i]->name;

            domain = dest->domains[dest->size];
            ++dest->size;
        }

        merge_fields(domain, src->domains[i]);
    }
}

/**
 * @brief Extracts the domains from the stored plugins and builds
 * a unified domain tree comprising all data associated with them.
 */
static void build_domain_tree(void)
{
    if (sdata.domains->size)
        clean_domain_tree();

    // Merges the domains present within the stored plugins into a
    // single struct.
    for (index_t i = 0; i < sdata.plugins->size; ++i)
    {
        PluginData *plugin = &sdata.plugins->plugins[i];

        // Skips if the plugin is disabled.
        if (!plugin->enabled)
            continue;

        merge_domains(sdata.domains, plugin->domains);
    }
}

/**
 * @brief Builds up the session data.
 */
SessionData *get_session_data(void)
{
    if (sdata.domains && sdata.plugins)
        return &sdata;

    // Allocates space for the domains and extracts the plugins.
    sdata = (SessionData){
        .domains = (Domains *)calloc(1, sizeof(Domains)),
        .plugins = get_plugins(),
    };

    build_domain_tree();
    return &sdata;
}

/**
 * @brief Toggles the current availability of the plugin.
 *
 * @details Toggles the plugin stored at the specified index in
 * the plugins array, and re-builds the domain tree if successful.
 *
 * @param inx Index of the plugin in the plugins array.
 * @return Boolean value signifying whether the switching operation
 * was successful.
 */
bool switch_plugin(index_t inx)
{
    if (toggle_plugin(inx))
    {
        build_domain_tree();
        return true;
    }

    return false;
}

/**
 * @brief Fress the memory allocated for the domain tree and
 *  plugins, and resets session data.
 */
void clean_session_data(void)
{
    clean_domain_tree();
    clean_plugins();

    free(sdata.domains->domains);
    free(sdata.domains);

    // Resets all members to the default state (NULL).
    memset(&sdata, 0, sizeof(SessionData));
}
