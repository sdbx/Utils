#ifndef ARENA_ADT_INTERNALS_H
#define ARENA_ADT_INTERNALS_H

#include <stdbool.h>

#include "common.h"
#include "wrappers.h"
#include "list.adt.h"
#include "arena.adt.h"

#define ARENA_SIZ_DEFAULT 8192

typedef struct {
   size_t cap;
   size_t pos;
   void *buf;
   /* tracks in order to optimize in-place realloc */
   void *last_alloc;
} arena_region_t;

struct arena {
   list_t *regions;
   arena_region_t *current;
};

static void destruct_region(void *item, size_t idx);
static bool need_new_region(arena_t *arena, size_t req_siz);
static void add_new_region(arena_t *arena, size_t req_siz);

#endif
