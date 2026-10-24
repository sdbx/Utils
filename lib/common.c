#include "common.h"

extern size_t min3(size_t a, size_t b, size_t c) {
   size_t t = a;
   if (b < t) t = b;
   if (c < t) t = c;
   return t;
}
