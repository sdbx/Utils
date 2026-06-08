#include "strutils.h"
#include "strutils.internals.h"

extern char **split(
   const char * restrict src,
   const char * restrict mark,
   size_t *retsiz
) {
   /*
    * An example input & output
    * mark = "**"
    * src  = "**a****b**"
    * arr  = ["", "a", "", "", "b", ""]
    */
   const int marklen = strlen(mark);

   size_t siz, max;
   const char *ini, *fin;
   char *buf, **arr;

   // Prepare an array of strings
   siz = 0;
   max = 2;
   arr = safe_malloc(max * sizeof arr[0]);
   ini = fin = src;

   for (;;) {
      // find the location of the next mark
      fin = strstr(fin, mark);

      if (!fin) {
         int rest = strlen(ini);
         buf = safe_malloc(rest + 1);
         strcpy(buf, ini);
      }
      else {
         ptrdiff_t diff = fin - ini;
         if (!diff) {
            buf = safe_malloc(1);
            buf[0] = '\0';
         }
         else {
            buf = safe_malloc(diff + 1);
            strncpy(buf, ini, diff);
            buf[diff] = '\0';
         }
      }

      // store the buffer into the array
      if (siz == max)
         arr = safe_realloc2x(arr, &max);
      arr[siz++] = buf;

      // update the states
      if (!fin)
         goto end;
      ini = fin = fin + marklen;
   }
   end: *retsiz = siz;

   return arr;
}
