#include "array.adt.h"
#include "array.adt.internals.h"

extern array_t *array_create(void) {
   array_t *a;

   a = safe_malloc(sizeof *a);
   a->arr = safe_malloc(INIT_MAX * ESIZ(a->arr));
   a->len = 0;
   a->max = INIT_MAX;

   return a;
}

extern void array_destroy(array_t *a) {
   for (size_t i = 0; i < a->len; i++)
      free(a->arr[i]);
   free(a->arr);
   free(a);
}

extern void *array_append(array_t *a, const void *item, size_t isiz) {
   void *p;

   if (array_full(a))
      array_enlarge(a);

   p = safe_malloc(isiz);
   memcpy(p, item, isiz);
   a->arr[a->len++] = p;

   return p;
}

extern void *array_get(array_t *a, int idx) {
   return a->arr[idx];
}

extern void array_set(array_t *a, int idx, void *item) {
   a->arr[idx] = item;
}

extern int array_size(array_t *a) {
   return a->len;
}

extern void array_foreach(array_t *a, array_iterator_t *iterate) {
   for (size_t i = 0; i < a->len; i++)
      (*iterate)(a->arr[i], i);
}

static inline bool array_full(array_t *a) {
   return a->len == a->max;
}

static void array_enlarge(array_t *a) {
   a->max *= 2;
   a->arr = safe_realloc2x_arr(
      a->arr,
      &a->max,
      ESIZ(a->arr)
   );
}
