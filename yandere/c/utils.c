#include "utils.h"

extern void seed(void) {
   srand(time(NULL));
}

extern int rand_range(int min, int max) {
   if (max <= 0)
      VERR("max must be greater than 0, but given %d", max);
   return min + rand() / (RAND_MAX / max + 1);
}

extern int mblen_(char ch) {
   unsigned char c;

   c = (unsigned char) ch;

   if (c < 0x80) return 1;             /* 0xxxxxxx */
   if ((c & 0xE0) == 0xC0) return 2;   /* 110xxxxx */
   if ((c & 0xF0) == 0xE0) return 3;   /* 1110xxxx */
   if ((c & 0xF8) == 0xF0) return 4;   /* 11110xxx */

   return 0;
}
