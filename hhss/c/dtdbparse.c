#include "dtdbparse.h"
#include "dtdbparse.internals.h"

extern array_t *dtdbparse(array_t *linestr) {
   array_t *res;
   line_t *l, temp;
   size_t siz;
   char *buf;

   res = array_create();
   siz = array_size(linestr);

   for (size_t i = 0; i < siz; i++) {
      l = array_get(linestr, i);

      if (l->run[0] == '#' ) continue;
      if (l->run[0] == '\n') continue;

      /* copy the whole line + \0 */
      buf = safe_malloc(l->len + 1);
      memcpy(buf, l->run, l->len + 1);

      temp.len = l->len;
      temp.num = l->num;
      temp.run = buf;

      array_append(res, &temp, sizeof temp);
   }

   return res;
}

extern void dtdbcheck(array_t *db) {
   int dbsiz, threshold;

   dbsiz = array_size(db);
   threshold = MIN_DATA_COUNT;

   if (dbsiz < threshold)
      VERR("data database must have at least "
         "%d entries but only %d",
         threshold, dbsiz
      );
}

extern void destroy_dtdb(array_t *dtdb) {
   int siz = array_size(dtdb);
   line_t *l;

   for (int i = 0; i < siz; i++) {
      l = array_get(dtdb, i);
      free(l->run);
   }

   array_destroy(dtdb);
}
