// Common header file providing declarations for all the
// architecture-dependent functionalities.

#ifndef _ARCH_H
#define _ARCH_H

#include "structs.h"

extern Domains *(**get_plugins(void))(void);
extern void clean(void);

#endif
