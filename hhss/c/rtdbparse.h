#ifndef RTDBPARSE_H
#define RTDBPARSE_H

/************
 * INCLUDES *
 ************/
#include "array.adt.h"

/**************
 * PROTOTYPES *
 **************/
array_t *rtdbparse(array_t *linestr);
array_t *rtdbquery(array_t *table, const char *sectname);
void rtdbcheck(array_t *db);
void destroy_rtdb(array_t *rtdb);

#endif
