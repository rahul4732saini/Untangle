#ifndef _SESSION_H
#define _SESSION_H

#include "shared.h"

SessionData *get_session_data(void);
void toggle_plugin(PluginData *plugin);
void clean_session_data(void);

#endif
