#include "replace.h"
#include "replace.internals.h"

extern void replace_templates(array_t *pts, array_t *rtdb) {
   array_t *pt, *rts, *sectarr;
   symbol_t *sym;
   size_t ptslen, ptlen, rtslen;
   char *rt;
   int v;

   pre_user = -1;
   ptslen = array_size(pts);

   for (size_t ptidx = 0; ptidx < ptslen; ptidx++) {
      pt = *((array_t **) array_get(pts, ptidx));
      ptlen = array_size(pt);

      for (size_t symidx = 0; symidx < ptlen; symidx++) {
         sym = array_get(pt, symidx);

         if (sym->kind == SymkindText)
            continue;

         rts = parse_replace_string(sym->content);
         rtslen = array_size(rts);

         if (rtslen == 0)
            synerr_empty();

         v = rand_range(0, rtslen);
         rt = *((char **) array_get(rts, v));

         sectarr = rtdbquery(rtdb, rt);
         if (!sectarr)
            synerr_invalid(rt);

         if (!strcmp(rt, "user"))
            rthandle_user(sym, sectarr);
         else
            rthandle_else(sym, sectarr);

         /* cleanup rts */
         for (size_t i = 0; i < rtslen; i++) {
            rt = *((char **) array_get(rts, i));
            free(rt);
         }
         array_destroy(rts);
      }
   }
}

static array_t *parse_replace_string(const char *rs) {
   static const char *delim = "|";
   char **raw;
   size_t rawsiz;
   array_t *ret;

   raw = split(rs, delim, &rawsiz);
   ret = array_create();

   for (size_t i = 0; i < rawsiz; i++) {
      if (raw[i][0] == '\0') {
         free(raw[i]);
         continue;
      }
      array_append(ret, &raw[i], sizeof raw[i]);
   }

   free(raw);

   return ret;
}

static void rthandle_user(symbol_t *sym, array_t *sectarr) {
   int r, siz;

   siz = array_size(sectarr);
   if (siz == 1) {
      r = 0;
      goto common;
   }

   for (;;) {
      r = rand_range(0, siz);
      if (r == pre_user)
         continue;
      pre_user = r;
      break;
   }

   common:
   rthandle_common(sym, sectarr, r);
}

static void rthandle_else(symbol_t *sym, array_t *sectarr) {
   int r, siz;

   siz = array_size(sectarr);
   r = rand_range(0, siz);
   rthandle_common(sym, sectarr, r);
}

static void rthandle_common(symbol_t *sym, array_t *sectarr, int idx) {
   char *val;

   val = array_get(sectarr, idx);
   free(sym->content);
   sym->content = safe_malloc(strlen(val) + 1);
   strcpy(sym->content, val);
}

static void synerr_invalid(const char *rt) {
   safe_fprintf(stderr,
      Cbred "<syntax error>" Creset " invalid replace template "
      Cemerald "%s" Creset "\n",
      rt
   );
   exit(EXIT_FAILURE);
}

static void synerr_empty(void) {
   safe_fprintf(stderr,
      Cbred "<syntax error>" Creset " empty replace template detected\n"
   );
   exit(EXIT_FAILURE);
}
