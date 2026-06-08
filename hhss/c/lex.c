#include "lex.h"
#include "lex.internals.h"

extern array_t *lex(array_t *charstrs) {
   array_t *tokstrs;
   char *charstr;
   size_t len;

   bufsiz = 128;
   buf = safe_malloc(bufsiz);
   pos = 0;

   len = array_size(charstrs);
   tokstrs = array_create();

   for (size_t i = 0; i < len; i++) {
      charstr = array_get(charstrs, i);
      lex_line(charstr);
      array_append(tokstrs, &tokstr, sizeof tokstr);
   }
   free(buf);

   return tokstrs;
}

static array_t *lex_line(const char *line) {
   tokstr = array_create();

   for (;;) {
      ch = *line++;
      switch (ch) {
         case '\0': tokenize(); goto done;
         case '$' : /* fall-through */
         case '{' : /* fall-through */
         case '}' : tokenize(); break;
         default  : charcopy();
      }
   }
   done:;

   return tokstr;
}

static void tokenize(void) {
   token_t tok;
   char *run;

   if (pos != 0) {
      buf[pos] = '\0';
      run = safe_malloc(pos + 1);
      strcpy(run, buf);
      tok.kind = Tokkind_chars;
      tok.run = run;
      tok.len = pos;
      array_append(tokstr, &tok, sizeof tok);
      pos = 0;
   }

   if (ch != '\0') {
      run = safe_malloc(2);
      run[0] = ch;
      run[1] = '\0';
      tok.kind = Tokkind_delim;
      tok.run = run;
      tok.len = 1;
      array_append(tokstr, &tok, sizeof tok);
   }
}

static void charcopy(void) {
   buf[pos++] = ch;
   if (pos == bufsiz)
      buf = safe_realloc2x(buf, &bufsiz);
}

extern void destroy_tokstrs(array_t *tokstrs) {
   int tokstrs_siz = array_size(tokstrs);
   int tokstr_siz;
   array_t *tokstr;
   token_t *tok;

   for (int i = 0; i < tokstrs_siz; i++) {
      tokstr = *((array_t **) array_get(tokstrs, i));
      tokstr_siz = array_size(tokstr);
      for (int k = 0; k < tokstr_siz; k++) {
         tok = array_get(tokstr, k);
         free(tok->run);
      }
      array_destroy(tokstr);
   }
   array_destroy(tokstrs);
}
