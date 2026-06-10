#ifndef UTILS_H
#define UTILS_H

/**********
 * MACROS *
 **********/
#define ESIZ(arr) (sizeof (arr)[0])
#
#define STREQL(s1,s2) (!(strcmp((s1),(s2))))

/**************
 * PROTOTYPES *
 **************/
void seed(void);
int rand_range(int min, int max);
int mblen_(char ch);

#endif
