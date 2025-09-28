// Common header file providing declarations for all the
// architecture-dependent functionalities.

#ifndef _PLATFORM_H
#define _PLATFORM_H

#include <stdbool.h>

#include "typedefs.h"
#include "shared.h"

extern PluginsData *get_plugins(void);
extern bool toggle_plugin(index_t inx);
extern void clean_plugins(void);

#endif
