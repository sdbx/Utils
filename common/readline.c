#include "readline.h"
#include "readline.internals.h"

static const char *errmsg = "unable to read";

extern int readln(FILE *fp, char **line, size_t *len) {
   int ch;
   size_t pos, bufsiz;
   char *buf;

   // First, tests whether there is nothing to read
   ch = getc(fp);
   if (ch == EOF) {
      if (ferror(fp)) ERR(errmsg);
      *line = NULL;
      *len = 0;
      return 1;
   }
   else ungetc(ch, fp);

   // Reads the file
   bufsiz = READLINE_UNIT;
   buf = safe_malloc(bufsiz);
   pos = 0;

   for (;;) {
      ch = getc(fp);
      if (ch == EOF) {
         if (ferror(fp)) ERR(errmsg);
         goto eol;
      }
      if (ch == '\r') continue;
      if (ch == '\n') goto eol;

      buf[pos++] = ch;
      if (pos == bufsiz)
         buf = safe_realloc2x(buf, &bufsiz);
   }

   eol: {
      if (bufsiz - pos == 1)
         buf = safe_realloc2x(buf, &bufsiz);
      buf[pos++] = '\n';
      buf[pos] = '\0';
      *line = buf;
      *len = pos;
      return 0;
   }
}
