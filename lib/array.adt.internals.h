#ifndef ARRAY_ADT_TYPES_H
#define ARRAY_ADT_TYPES_H

/************
 * INCLUDES *
 ************/
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#
#include "utils.h"
#include "wrappers.h"
#include "array.adt.h"

/**********
 * MACROS *
 **********/
#define INIT_MAX 128

/************
 * TYPEDEFS *
 ************/
struct array {
   size_t len, max;
   void **arr;
};

/**************
 * PROTOTYPES *
 **************/
static inline bool array_full(array_t *a);
static void array_enlarge(array_t *arr);

#endif
