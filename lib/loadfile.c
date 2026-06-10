#include "loadfile.h"
#include "loadfile.internals.h"

extern array_t *loadfile(
   const char *filename,
   int * restrict lc,
   int * restrict cc
) {
   FILE *fp;
   array_t *lines;
   line_t line;
   int lcnt,  // line count
       ccnt;  // character count

   fp = safe_fopen(filename, "r");
   lines = array_create();
   lcnt = ccnt = 0;

   while (!readln(fp, &line.run, &line.len)) {
      line.num = ++lcnt;
      ccnt += line.len;
      array_append(lines, &line, sizeof line);
   }

   if (lcnt == 0)
      ERR("empty source file");
   safe_fclose(fp);

   if (lc) *lc = lcnt;
   if (cc) *cc = ccnt;
   return lines;
}

extern void unloadfl(array_t *lines) {
   int siz = array_size(lines);

   for (int i = 0; i < siz; i++)
      free(((line_t *) array_get(lines, i))->run);

   array_destroy(lines);
}
