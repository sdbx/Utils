#ifndef COMMON_H
#define COMMON_H

#include <stddef.h>

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

#define ESIZ(arr) (sizeof (arr)[0])
#define ARRLEN(arr) (sizeof (arr) / ESIZ(arr))

size_t min3(size_t a, size_t b, size_t c);

#endif
