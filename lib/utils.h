#ifndef UTILS_H
#define UTILS_H

/**********
 * MACROS *
 **********/
#define ESIZ(arr) (sizeof (arr)[0])

/**************
 * PROTOTYPES *
 **************/
void seed(void);
int rand_range(int min, int max);
int mblen_(char ch);

#endif
