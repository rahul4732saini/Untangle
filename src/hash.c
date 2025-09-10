#include <stdint.h>
#include <string.h>

#include "typedefs.h"
#include "shared.h"

typedef uint32_t hash_t;

typedef struct bucket_node
{
    void *data;
    struct bucket_node *next;
} BucketNode;

typedef struct
{
    BucketNode **buckets;
    len_t size;
    char *(*key_extractor)(void *);
} HashTable;

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
        .buckets = (BucketNode *)calloc(size, sizeof(BucketNode)),
        .size = size,
        .key_extractor = key_extractor,
    };

    return table;
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
