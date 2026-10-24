#ifndef ARENA_ADT_H
#define ARENA_ADT_H

#include <string.h>
#include <stddef.h>

#include "wrappers.h"

typedef struct arena arena_t;

arena_t *arena_create(void);
void arena_info(const char *execname);
void arena_cleanup(arena_t *arena);
void arena_destroy(arena_t *arena);
void *arena_malloc(arena_t *arena, size_t siz);
void *arena_calloc(arena_t *arena, size_t nmemb, size_t siz);
inline void *arena_realloc2x(arena_t *arena, void *ptr, size_t *siz);
void *arena_realloc2x_arr(
   arena_t *arena,
   void *ptr,
   size_t *nmemb,
   size_t esiz
);
inline void *arena_malloc_fallback(arena_t *arena, size_t siz);
inline void *arena_realloc2x_arr_fallback(
   arena_t *arena,
   void *ptr,
   size_t *cnt,
   size_t esiz
);
inline void arena_free_fallback(arena_t *arena, void *ptr);

/*====================*
 | INLINE DEFINITIONS |
 *====================*/
inline void *arena_malloc_fallback(arena_t *arena, size_t siz) {
   return arena ? arena_malloc(arena, siz) : safe_malloc(siz);
}

inline void *arena_calloc_fallback(arena_t *arena, size_t nmemb, size_t siz) {
   return arena ? arena_calloc(arena, nmemb, siz) : safe_calloc(nmemb, siz);
}

inline void *arena_realloc2x(arena_t *arena, void *ptr, size_t *siz) {
   return arena_realloc2x_arr(arena, ptr, siz, sizeof (char));
}

inline void *arena_realloc2x_arr_fallback(
   arena_t *arena,
   void *ptr,
   size_t *cnt,
   size_t esiz
) {
   return arena
      ? arena_realloc2x_arr(arena, ptr, cnt, esiz)
      : safe_realloc2x_arr(ptr, cnt, esiz);
}

inline void arena_free_fallback(arena_t *arena, void *ptr) {
   if (arena == NULL) free(ptr);
   /* if arena != NULL, no-op */
}

#endif
