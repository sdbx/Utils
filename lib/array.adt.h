#ifndef ARRAY_ADT_H
#define ARRAY_ADT_H

#include <stddef.h>

/*==========*
 | TYPEDEFS |
 *===========*/
typedef struct array array_t;
typedef void array_iterator_t(void *item, size_t idx, void *ctx);
typedef int array_sorter_t(const void *d1, const void *d2);
typedef void array_destructor_t(void *item, size_t idx);

/*============*
 | PROTOTYPES |
 *============*/
array_t *array_create(array_destructor_t *destruct);
void array_destroy(array_t *a);
void array_append(array_t *a, const void *item, size_t siz);
void *array_peek(array_t *a, int idx);
void array_set(array_t *a, int idx, void *item);
void array_foreach(array_t *a, array_iterator_t *iterate, void *ctx);
size_t array_size(array_t *a);
void array_sort(array_t *a, array_sorter_t *compare);

#endif
