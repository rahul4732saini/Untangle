#include <stdint.h>

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
