#ifndef STRUTILS_H
#define STRUTILS_H

/************
 * INCLUDES *
 ************/
#include <stddef.h>
#
#include "utils.h"

/**************
 * PROTOTYPES *
 **************/
char **split(
   const char * restrict src,
   const char * restrict mark,
   size_t *retsiz
);

#endif
