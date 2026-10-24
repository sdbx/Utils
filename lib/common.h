#ifndef COMMON_H
#define COMMON_H

#include <stddef.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <inttypes.h>

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

#define ESIZ(arr) (sizeof (arr)[0])
#define ARRLEN(arr) (sizeof (arr) / ESIZ(arr))

#define STREQL(s1,s2) (!(strcmp((s1),(s2))))

bool isle(void);
size_t min(size_t a, size_t b);
size_t min3(size_t a, size_t b, size_t c);
void seed(void);
int rand_range(int min, int max);
int mblen_(char ch);

inline uint32_t endrev32(uint32_t v) {
   return v << 24
         | (v & 0x0000FF00) << 8
         | (v & 0x00FF0000) >> 8
         | (v & 0xFF000000) >> 24;
}

#endif
