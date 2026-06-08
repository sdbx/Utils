#ifndef PRELEX_H
#define PRELEX_H

/************
 * INCLUDES *
 ************/
#include "array.adt.h"

/**************
 * PROTOTYPES *
 **************/
array_t *prelex(array_t *db, int datcnt);
void destroy_charstrs(array_t *charstrs);

#endif
