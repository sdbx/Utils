#ifndef ARR_ADT_H
#define ARR_ADT_H

/************
 * INCLUDES *
 ************/
#include <stddef.h>
#include <string.h>

/************
 * TYPEDEFS *
 ************/
typedef struct array array_t;
typedef void array_iterator_t(void *item, int idx);

/**************
 * PROTOTYPES *
 **************/
array_t *array_create(void);
void array_destroy(array_t *a);
void *array_append(array_t *a, const void *item, size_t isiz);
void *array_get(array_t *a, int idx);
void array_set(array_t *a, int idx, void *item);
int array_size(array_t *a);
void array_foreach(array_t *a, array_iterator_t *iterate);

#endif
