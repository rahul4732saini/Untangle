#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "typedefs.h"
#include "shared.h"

/**
 * @brief Extracts the hash of the string using DJB2 hashing algorithm.
 * @param src Pointer to the source stirng.
 * @return The hash of the string.
 */
static hash_t get_hash(char *src)
{
    hash_t result = 5381;

    for (; *src; ++src)
        result = (result << 5) + result + *src;

    return result;
}

/**
 * @brief Initializes a hash table based on the given specifications.
 * @param size Number of buckets to be present in the hash table.
 * @param key_extractor Function to extract the key from table data.
 */
HashTable *init_hash_table(len_t size, char *(*key_extractor)(void *))
{
    HashTable *table = (HashTable *)malloc(sizeof(HashTable));

    *table = (HashTable){
        .buckets = (BucketNode **)calloc(size, sizeof(BucketNode *)),
        .size = size,
        .key_extractor = key_extractor,
    };

    return table;
}

/**
 * @brief Stores the given data into the hash table.
 * @param table Pointer to the HashTable struct comprising the
 * hash table data.
 * @param data Pointer to the data to be stored in the table.
 */
void add_table_data(HashTable *table, void *data)
{
    BucketNode *bucket = (BucketNode *)malloc(sizeof(BucketNode));
    bucket->data = data;

    hash_t hash = get_hash(table->key_extractor(data));
    index_t pos = hash % table->size;

    *bucket = (BucketNode){
        .data = data,
        .next = table->buckets[pos],
    };

    table->buckets[pos] = bucket;
}

/**
 * @details Extracts the data from the hash table by matching the keys.
 * @param Pointer to the HashTable struct comprising the hash table
 * data.
 * @param key The string whose hash will be used for finding the data.
 * @return A pointer to the data is returned if found, or NULL to
 * indicate failure in lookup.
 */
void *get_table_data(HashTable *table, char *key)
{
    hash_t hash = get_hash(key);
    index_t pos = hash % table->size;

    for (
        BucketNode *bucket = table->buckets[pos];
        bucket;
        bucket = bucket->next)
    {
        if (!strcmp(key, table->key_extractor(bucket->data)))
            return bucket->data;
    }

    return NULL;
}

/**
 * @brief Frees the resources allocated for the hash table.
 * @param table Pointer the HashTable struct comprising the hash
 * table data.
 */
void free_hash_table(HashTable *table)
{
    // Frees the resources allocated for each bucket node.
    for (len_t i = 0; i < table->size; ++i)
    {
        while (table->buckets[i])
        {
            BucketNode *bucket = table->buckets[i];
            table->buckets[i] = table->buckets[i]->next;
            free(bucket);
        }
    }

    free(table->buckets);
    free(table);
}
