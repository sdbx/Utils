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
sigprocessor_t sighandle_sigint;
sigprocessor_t sighandle_sigquit;
sigprocessor_t sighandle_sigterm;

#endif
