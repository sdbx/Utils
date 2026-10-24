#include "loadfile.h"
#include "loadfile.internals.h"

extern array_t *loadfile(
   const char *filename,
   size_t * restrict lc,
   size_t * restrict wc
) {
   FILE *fp;
   array_t *lines;
   line_t line;
   size_t rlen,   /* the length of `run` */
          lcnt,   /* line count */
          wcnt;   /* word count */
   char *run;  /* the contents of a line */

   fp = safe_fopen(filename, "r");
   lcnt = wcnt = 0;
   lines = array_create(destruct_line);

   while (!readln(fp, &run, &rlen)) {
      line.len = rlen;
      line.run = run;
      line.num = ++lcnt;
      wcnt += rlen;
      array_append(lines, &line, sizeof line);
   }

   if (lcnt == 0) {
      ERR("empty source file");
   }

   safe_fclose(fp);

   if (lc != NULL) *lc = lcnt;
   if (wc != NULL) *wc = wcnt;
   return lines;
}

extern void unloadfl(array_t *lines) {
   int siz = array_size(lines);

   for (int i = 0; i < siz; i++)
      free(((line_t *) array_peek(lines, i))->run);

   array_destroy(lines);
}

static void destruct_line(void *item, size_t idx) {
   line_t *l;

   (void) idx;

   l = item;
   free(l->run);
}
