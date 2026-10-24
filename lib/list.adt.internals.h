#ifndef LIST_ADT_INTERNALS_H
#define LIST_ADT_INTERNALS_H

#include <stdlib.h>
#include <string.h>

#include "wrappers.h"
#include "list.adt.h"

/*
`node_t` is a type for representing an element which
constitutes a list.

`prev` is the pointer to the previous element.
`next` is the pointer to the next element.
`data` is the pointer to an object.
`data_size` is the size of the object pointed by `data`.
*/
typedef struct node {
   struct node *prev;
   struct node *next;
   void *data;
   size_t data_size;
} node_t;

/*
`struct list` is the actual definition of the `list_t` type declared in `list.h`.

`head` is the pointer to the first element.
`tail` is the pointer to the last element.
`size` is the length of this list.
*/
struct list {
   node_t *head;
   node_t *tail;
   size_t size;
   list_destructor_t *destruct;
};

/**
 * @brief `make_node` does the following tasks:
 *    1. It makes a `node_t` object.
 *    2. It copies the object pointed by `item`.
 *    3. It stores the copied data into the node.
 *    4. It returns the node.
 * @param item a pointer to an object to copy from; it must not be NULL.
 * @param item_size the size of the object to be copied; it must not be 0.
 * @return a pointer to a `node_t` object whose data is copied from `item`.
 */
static node_t *make_node(void *item, size_t item_size);

/**
 * @brief `get_node` finds the `index`th node.
 * @param list a list to search for; it must not be NULL.
 * @param index range: 0 <= index < list.length
 * @return the pointer to the node; if the list is empty, it returns NULL.
 */
static node_t *get_node(list_t *list, size_t index);

#endif
