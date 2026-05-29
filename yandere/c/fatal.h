#ifndef FATAL_H
#define FATAL_H

/************
 * INCLUDES *
 ************/
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#
#include "global.h"
#include "wrappers.h"
#include "colorcodes.h"

/**********
 * MACROS *
 **********/
#define ERR(errmsg) fatal(errmsg, __func__)
#define VERR(errmsg, ...) vfatal(errmsg, __func__, __VA_ARGS__)

/**************
 * PROTOTYPES *
 **************/
void fatal(const char *errmsg, const char *funcname);
void vfatal(const char *errmsg, const char *funcname, ...);

#endif
