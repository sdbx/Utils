#ifndef DEBUG_H
#define DEBUG_H

/************
 * INCLUDES *
 ************/
#include "array.adt.h"

/**************
 * PROTOTYPES *
 **************/
void debug_rtdb(array_t *rtdb);
void debug_charstrs(array_t *charstrs);
void debug_tokstr(array_t *tokstr);
void debug_tokstrs(array_t *tokstrs);
void debug_pts(array_t *pts);

#endif
