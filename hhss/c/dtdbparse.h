#ifndef DTDBPARSE_H
#define DTDBPARSE_H

/************
 * INCLUDES *
 ************/
#include "array.adt.h"

/**************
 * PROTOTYPES *
 **************/
array_t *dtdbparse(array_t *linestr);
void dtdbcheck(array_t *db);
void destroy_dtdb(array_t *dtdb);

#endif
