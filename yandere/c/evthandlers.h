#ifndef EVTHANDLERS_H
#define EVTHANDLERS_H

/************
 * INCLUDES *
 ************/
#include <stdio.h>
#include <stdlib.h>
#
#include "noise.h"
#include "global.h"

/**************
 * PROTOTYPES *
 **************/
void evthandle_default(void);
void evthandle_sigint(void);
void evthandle_sigquit(void);
void evthandle_sigterm(void);

#endif
