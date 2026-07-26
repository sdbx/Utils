#include "prelex.h"
#include "prelex.internals.h"

extern array_t *prelex(array_t *db, int datcnt) {
   array_t *ret;
   line_t *curr, *last;
   int lastpos, pickcnt, rv;

   ret = array_create();
   lastpos = array_size(db) - 1;  /* last elem idx = len - 1 */

   pickcnt = datcnt - 1;  /* -1 since the last pick won't swap */
   /* pick & swap (datcnt - 1) times */
   for (int k = 0; k < pickcnt; k++) {
      rv = rand_range(0, lastpos);  /* [0, lp] */

      curr = array_get(db, rv);
      last = array_get(db, lastpos);

      // swap
      array_set(db, lastpos, curr);
      array_set(db, rv, last);

      array_append(ret, curr->run, curr->len + 1);

      // in order to prevent duplication
      lastpos--;
   }
   rv = rand_range(0, lastpos);  /* the last pick (no swap) */
   curr = array_get(db, rv);
   array_append(ret, curr->run, curr->len + 1);

   return ret;
}

extern void destroy_charstrs(array_t *charstrs) {
   array_destroy(charstrs);
}
