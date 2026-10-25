#include "array.adt.h"
#include "array.adt.internals.h"

extern array_t *array_create(array_destructor_t *destruct) {
   array_t *a;

   a = safe_malloc(sizeof *a);
   a->container = safe_malloc(INIT_MAX * ESIZ(a->container));
   a->len = 0;
   a->max = INIT_MAX;
   a->destruct = destruct;

   return a;
}

extern void array_destroy(array_t *a) {
   for (size_t i = 0; i < a->len; i++) {
      if (a->destruct != NULL)
         a->destruct(a->container[i], i);
      free(a->container[i]);
   }
   free(a->container);
   free(a);
}

extern void array_append(array_t *a, const void *item, size_t siz) {
   void *p;

   if (array_full(a)) array_enlarge(a);

   p = safe_malloc(siz);
   memcpy(p, item, siz);
   a->container[a->len++] = p;
}

extern void *array_peek(array_t *a, int idx) {
   return a->container[idx];
}

extern void array_set(array_t *a, int idx, void *item) {
   a->container[idx] = item;
}

extern void array_foreach(array_t *a, array_iterator_t *iterate, void *ctx) {
   for (size_t i = 0; i < a->len; i++) {
      (*iterate)(a->container[i], i, ctx);
   }
}

extern size_t array_size(array_t *a) {
   return a->len;
}

extern void array_sort(array_t *a, array_sorter_t *compare) {
   qsort(a->container, a->len, ESIZ(a->container), compare);
}

static inline bool array_full(array_t *a) {
   return a->len == a->max;
}

static void array_enlarge(array_t *a) {
   a->container = safe_realloc2x_arr(
      a->container,
      &a->max,
      ESIZ(a->container)
   );
}
