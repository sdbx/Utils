#ifndef UTILS_H
#define UTILS_H

/************
 * INCLUDES *
 ************/
#include <time.h>
#include <stdlib.h>
#
#include "fatal.h"
#include "global.h"

/**************
 * PROTOTYPES *
 **************/
void seed(void);
int rand_range(int min, int max);
int mblen_(char ch);

#endif
