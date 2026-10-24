#include "readline.h"
#include "readline.internals.h"

extern int readln(FILE *fp, char **line, size_t *len) {
   int ch;
   size_t pos, bufsiz;
   char *buf;

   /* First of all, checks whether there is nothing to read */
   ch = safe_fgetc(fp);

   if (ch == EOF) {
      *line = NULL;
      *len = 0;
      return 1;
   }
   else ungetc(ch, fp);

   bufsiz = READLINE_UNIT;
   buf = safe_malloc(bufsiz);
   pos = 0;

   /* Reads the file, one line at a time */
   for (;;) {
      ch = safe_fgetc(fp);

      if (ch == EOF || ch == '\n')
         break;
      if (ch == '\r')
         continue;

      buf[pos] = ch;

      if (bufsiz - pos == 1) {
         buf = safe_realloc2x(buf, &bufsiz);
      }

      pos++;
   }

   /* Stores \n\0 at the end */
   if (bufsiz - pos == 1) {
      buf = safe_realloc2x(buf, &bufsiz);
   }

   buf[pos++] = '\n';
   buf[pos] = '\0';

   *line = buf;
   if (len != NULL) *len = pos;
   return 0;
}
