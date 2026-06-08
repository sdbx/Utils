#include "debug.h"
#include "debug.internals.h"

extern void debug_rtdb(array_t *rtdb) {
   typedef struct {
      char *sect;
      array_t *addr;
   } mapper_t;

   mapper_t *m;
   char *s;
   size_t siz, siz2;
   size_t i, k;

   siz = array_size(rtdb);

   for (i = 0; i < siz; i++) {
      m = array_get(rtdb, i);
      printf("rtdb[%zu]\n", i);
      printf(".sectname = [%s]\n", m->sect);
      printf(".sectarr\n");
      siz2 = array_size(m->addr);
      for (k = 0; k < siz2; k++) {
         s = array_get(m->addr, k);
         printf("  [%zu] = [%s]\n", k, s);
      }
   }
}

extern void debug_charstrs(array_t *charstrs) {
   char *s;
   size_t i, siz;

   siz = array_size(charstrs);
   printf("charstrs.len = %zu\n", siz);

   for (i = 0; i < siz; i++) {
      s = array_get(charstrs, i);
      printf("charstrs[%zu] = [%s]\n", i, s);
   }
}

extern void debug_tokstr(array_t *tokstr) {
   token_t *tok;
   size_t k, siz;

   siz = array_size(tokstr);
   printf("tokstr.len = %zu\n", siz);

   for (k = 0; k < siz; k++) {
      tok = array_get(tokstr, k);
      printf("tokstr[%zu]\n", k);
      printf("  .kind = [%d]\n", tok->kind);
      printf("  .run = [%s]\n", tok->run);
      printf("  .len = [%d]\n", tok->len);
   }

   printf("\n");
}

extern void debug_tokstrs(array_t *tokstrs) {
   array_t *tokstr;
   token_t *tok;
   size_t siz, siz2;
   size_t i, k;

   siz = array_size(tokstrs);
   printf("tokstrs.len = %zu\n", siz);

   for (i = 0; i < siz; i++) {
      tokstr = *((array_t **) array_get(tokstrs, i));
      siz2 = array_size(tokstr);
      printf("tokstrs[%zu]\n", i);
      printf("tokstrs[%zu].len = %zu\n", i, siz2);

      for (k = 0; k < siz2; k++) {
         tok = array_get(tokstr, k);
         printf("  tokstrs[%zu][%zu]\n", i, k);
         printf("    .kind = [%d]\n", tok->kind);
         printf("    .run = [%s]\n", tok->run);
         printf("    .len = [%d]\n", tok->len);
      }
   }
}

extern void debug_pts(array_t *pts) {
   array_t *pt;
   symbol_t *sym;
   size_t siz, siz2;
   size_t i, k;

   siz = array_size(pts);
   printf("pts.len = %zu\n", siz);

   for (i = 0; i < siz; i++) {
      pt = *((array_t **) array_get(pts, i));
      siz2 = array_size(pt);
      printf("pts[%zu]\n", i);
      printf("pts[%zu].len = %zu\n", i, siz2);

      for (k = 0; k < siz2; k++) {
         sym = array_get(pt, k);
         printf("  pts[%zu][%zu]\n", i, k);
         printf("    .kind = [%d]\n", sym->kind);
         printf("    .run = [%s]\n", sym->content);
      }
   }
}
