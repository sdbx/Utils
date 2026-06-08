#include "replace.h"
#include "replace.internals.h"

extern void replace_templates(array_t *pts, array_t *rtdb) {
   array_t *pt;
   symbol_t *sym;
   char **rts, *rt;
   size_t ptslen, ptlen;
   size_t rtslen;
   array_t *sectarr;

   pre_user = 0;
   ptslen = array_size(pts);

   for (size_t ptidx = 0; ptidx < ptslen; ptidx++) {
      pt = *((array_t **) array_get(pts, ptidx));
      ptlen = array_size(pt);

      for (size_t symidx = 0; symidx < ptlen; symidx++) {
         sym = array_get(pt, symidx);

         if (sym->kind == Symkind_text)
            continue;

         rts = split(sym->content, "|", &rtslen);
         rt = rts[rand_range(0, rtslen)];

         sectarr = rtdbquery(rtdb, rt);
         if (!sectarr)
            synerr(rt);

         if (!strcmp(rt, "user"))
            rthandle_user(sym, sectarr);
         else
            rthandle_else(sym, sectarr);

         // cleanup
         for (size_t i = 0; i < rtslen; i++)
            free(rts[i]);
         free(rts);
      }
   }
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

static void synerr(const char *rt) {
   safe_fprintf(stderr,
      Cbred "<syntax error>" Creset " invalid replace template "
      Cemerald "%s" Creset "\n",
      rt
   );
   exit(EXIT_FAILURE);
}
