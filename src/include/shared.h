#ifndef _SHARED_H
#define _SHARED_H

#include "structs.h"
#include "typedefs.h"

typedef struct
{
    Domains *(**functions)(void);
    len_t size;
} PluginFunctions;

#endif
