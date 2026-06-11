#ifndef LOADFILE_H
#define LOADFILE_H

/************
 * INCLUDES *
 ************/
#include "array.adt.h"

/************
 * TYPEDEFS *
 ************/
typedef struct line {
   char *run;
   size_t len, num;
} line_t;

array_t *loadfile(
   const char *filename,
   int * restrict lc,
   int * restrict cc
);

void unloadfl(array_t *lines);

#endif
