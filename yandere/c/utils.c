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
   int len, push, temp;

   /* check if 0xxxxxxx */
   if (0 <= ch)
      return 1;

   /*
    * handle 3 cases:
    *    110xxxxx => return 2
    *    1110xxxx => return 3
    *    11110xxx => return 4
    */
   len = 0;
   push = 7;
   do {
      temp = ch >> push;
      temp &= 0x1;
      if (temp == 0x1)
         len++;
      else break;
   } while (--push);

   return len;
}
