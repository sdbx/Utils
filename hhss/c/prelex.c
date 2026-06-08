#include "prelex.h"
#include "prelex.internals.h"

extern array_t *prelex(array_t *db, int datcnt) {
   array_t *ret;
   line_t *curr, *last;
   int lastpos, rv;

   ret = array_create();
   lastpos = array_size(db);

   for (int k = 0; k < datcnt; k++) {
      rv = rand_range(0, lastpos);  /* [0, lp) */

      curr = array_get(db, rv);
      last = array_get(db, lastpos - 1);

      // swap
      array_set(db, lastpos - 1, curr);
      array_set(db, rv, last);

      array_append(ret, curr->run, curr->len + 1);

      // in order to prevent duplication
      lastpos--;
   }

   return ret;
}

extern void destroy_charstrs(array_t *charstrs) {
   array_destroy(charstrs);
}
