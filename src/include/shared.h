#ifndef _SHARED_H
#define _SHARED_H

#include "structs.h"
#include "typedefs.h"

typedef struct
{
    char *name;
    Domains *(*function)(void);
} PluginData;

typedef struct
{
    PluginData *plugins;
    len_t size;
} PluginsData;

#endif
