#ifndef SIGHANDLERS_H
#define SIGHANDLERS_H

/************
 * INCLUDES *
 ************/
#include "global.h"
#include "wrappers.h"

/**************
 * PROTOTYPES *
 **************/
sighandler_t sighandle_sigint;
sighandler_t sighandle_sigquit;
sighandler_t sighandle_sigterm;

#endif
