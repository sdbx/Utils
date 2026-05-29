#ifndef YANDERE_TYPES_H
#define YANDERE_TYPES_H

/************
 * INCLUDES *
 ************/
#include "utils.h"
#include "global.h"
#include "wrappers.h"
#include "sighandlers.h"
#include "evthandlers.h"

/**************
 * PROTOTYPES *
 **************/
static void install_sighandlers(void);
static void dispatch(void);

#endif
