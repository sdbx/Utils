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
   max = 4;
   arr = safe_malloc(max * ESIZ(arr));
   ini = fin = src;

   do {
      // Find the location of the next mark
      fin = strstr(fin, mark);

      /* no mark found */
      if (!fin) {
         /* copy the rest of the string including \0 */
         size_t rest = strlen(ini);
         buf = safe_malloc(rest + 1);
         memcpy(buf, ini, rest + 1);
      }
      /* found */
      else {
         ptrdiff_t diff = fin - ini;

         /* fin points to the mark found */
         if (!diff) {
            /* store \0 */
            buf = safe_malloc(1);
            buf[0] = '\0';

            /* skips the mark */
            ini = fin = fin + marklen;
         }
         /* there is a string before the mark found */
         else {
            /* store the string */
            buf = safe_malloc(diff + 1);
            strncpy(buf, ini, diff);
            buf[diff] = '\0';

            /* moves to the mark */
            ini = fin;
         }
      }

      // Store the buffer into the array
      if (siz == max)
         arr = safe_realloc2x_arr(arr, &max, ESIZ(arr));
      arr[siz++] = buf;

   } while (fin);

   *retsiz = siz;
   return arr;
}
