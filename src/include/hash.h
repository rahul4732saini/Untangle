#ifndef _HASH_H
#define _HASH_H

#include "typedefs.h"
#include "shared.h"

HashTable *init_hash_table(len_t size, char *(*)(void *));
void add_table_data(HashTable *table, void *data);
void *get_table_data(HashTable *table, char *key);
void free_hash_table(HashTable *table);

#endif
