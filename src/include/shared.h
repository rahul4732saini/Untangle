#ifndef _SHARED_H
#define _SHARED_H

#include <stdbool.h>

#include "structs.h"
#include "typedefs.h"

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

#endif
