#ifndef _SESSION_H
#define _SESSION_H

#include <stdbool.h>

#include "typedefs.h"
#include "shared.h"

SessionData *get_session_data(void);
bool switch_plugin(index_t inx);
void clean_session_data(void);

#endif
