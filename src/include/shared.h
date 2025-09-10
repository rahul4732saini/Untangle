#ifndef _SHARED_H
#define _SHARED_H

#include <stdbool.h>

#include "structs.h"
#include "typedefs.h"

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

typedef struct
{
    char *name;
    Domains *domains;
    bool enabled;
} PluginData;

typedef struct
{
    PluginData *plugins;
    len_t size;
} PluginsData;

typedef struct
{
    PluginsData *plugins;
    Domains *domains;
} SessionData;

#endif
