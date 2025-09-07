#ifndef _SHARED_H
#define _SHARED_H

#include "structs.h"
#include "typedefs.h"

typedef struct
{
    char *name;
    Domains *(**functions)(void);
    len_t size;
} PluginData;

#endif
