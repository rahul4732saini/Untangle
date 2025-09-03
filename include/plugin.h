#ifndef _SHARED_PLUGIN_H
#define _SHARED_PLUGIN_H

#include <stdint.h>

typedef struct
{
    const char **fields;
    uint32_t size;
} Fields;

#endif