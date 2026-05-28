#include "noise.h"

extern void noise(const char *msg) {
   int i, k;
   int rv, chlen;
   bool overflow;

   for (i = k = 0; msg[i] != '\0'; /* empty */) {
      rv = rand_range(0, 6);
      chlen = mblen_(msg[i]);
      overflow = (k + chlen) >= (BUFMAX - 1);

      if (overflow) break;
      if (rv == 0) {   /* 1/6 chance */
         buf[k++] = *("#?@" + rand_range(0, 3));
         i += chlen;
      }
      else for (int m = 0; m < chlen; m++)
         buf[k++] = msg[i++];
   }
   buf[k] = '\0';
}
