#ifndef LIST_ADT_H
#define LIST_ADT_H

#include <stddef.h>
#include <stdbool.h>

/* `list_t` is a data type for list data structure.

`list.adt.h` provides the interface of this type.
`list.adt.c` contains the actual definitions of this type
and the functions for handling a list.

Meanwhile, since this type is designed to be an abstract
data type, the header file only provides this incomplete
type in order to hide its actual implementation. */
typedef struct list list_t;

/* `list_statcode_t` is an enumeration type for indicating
whether an operation on a list has been successful
or not.

`ListStatcodeFailed` represents a failure on an operation.
`ListStatcodeOk` represents a success. */
typedef enum list_statcode {
   ListStatcodeFailed,
   ListStatcodeOk
} list_statcode_t;

typedef void list_destructor_t(void *item, size_t idx);

/* < Table of Contents >

The following functions are the operations for
handling a list.

Note: HEAD means the first item of a list, meanwhile
      TAIL means the last item of the list.

1. Initialization
   1.1. list_create    : makes a new list
   1.2. list_destroy   : destroys a list
   1.3. list_cleanup   : deletes every item in a list
2. Insertion
   2.1. list_push      : inserts to TAIL
   2.2. list_unshift   : inserts to HEAD
   2.3. list_insert    : inserts to any possible position
3. Removal
   3.1. list_pop       : removes from TAIL
   3.2. list_shift     : removes from HEAD
   3.3. list_erase     : removes from any possible position
4. Retrieval
   4.1. list_tail      : refers to TAIL
   4.2. list_head      : refers to HEAD
   4.3. list_peek      : refers to any possible position
   4.4. list_foreach   : fixme: write here
5. Status Inquiry
   5.1. list_size      : gets the size of a list
   5.2. list_empty     : checks whether a list is empty
   5.3. list_full      : checks whether a list is full
   5.4. list_indexof   : gets the index of an item */

/**
 * @brief `list_create` makes a new list.
 * @return the pointer to the list having been created.
 * @note In failure, this function stops the program.
 */
list_t *list_create(list_destructor_t *destruct);

/**
 * @brief `list_cleanup` deletes every item in the `list`. In other words, it frees memory allocated for items.
 * @param list a list to delete all items inside it
 */
void list_cleanup(list_t *list);

/**
 * @brief `list_destroy` does what its name implies.
 * @param list a list to destroy
 */
void list_destroy(list_t *list);

/**
 * @brief `list_push` copies `item` of size `item_size` and has the copied item as the TAIL of `list`.
 * @param list a list to store a copied item
 * @param item an item to copy from
 * @param item_size the size of the item
 * @return the pointer to the copied item
 * @note In failure, this function stops the program.
 */
void *list_push(list_t *list, void *item, size_t item_size);

/**
 * @brief `list_unshift` copies `item` of size `item_size` and has the copied item as the HEAD of `list`.
 * @param list a list to store a copied item
 * @param item an item to copy from
 * @param item_size the size of the item
 * @return the pointer to the copied item
 * @note In failure, this function stops the program.
 */
void *list_unshift(list_t *list, void *item, size_t item_size);

/**
 * @brief `list_insert` copies `item` of size `item_size` and has the copied item as the `index`th element of `list`, i.e. this function inserts the `item` next to the `index - 1`th item.
 * @param list a list to store a copied item
 * @param index range: 0 <= index < list.length
 * @param item an item to copy from
 * @param item_size the size of the item
 * @return the pointer to the copied item
 * @note In failure, this function stops the program.
 */
void *list_insert(list_t *list, size_t index, void *item, size_t item_size);

/**
 * @brief `list_pop` removes an item from the TAIL of `list` and stores the item to the object pointed by `dest`.
 * @param list a list to remove an item from
 * @param dest a pointer to store the data
 * @return a status code indicating whether this operation has been successful
 * @note If `dest` is `NULL`, it doesn't store the item.
 */
list_statcode_t list_pop(list_t *list, void *dest);

/**
 * @brief `list_shift` removes an item from the HEAD of `list` and stores the item to the object pointed by `dest`.
 * @param list a list to remove an item from
 * @param dest a pointer to store the data
 * @return a status code indicating whether this operation has been successful
 * @note If `dest` is `NULL`, it doesn't store the item.
 */
list_statcode_t list_shift(list_t *list, void *dest);

/**
 * @brief `list_erase` removes an item from the `index`th position of `list` and stores the item to the object pointed by `dest`.
 * @param list a list to remove an item from
 * @param index range: 0 <= index < list.length
 * @param dest a pointer to store the data
 * @return a status code indicating whether this operation has been successful
 * @note If `dest` is `NULL`, it doesn't store the item.
 */
list_statcode_t list_erase(list_t *list, size_t index, void *dest);

/**
 * @brief `list_tail` searches for the TAIL.
 * @param list a list to search
 * @return the pointer to the TAIL
 */
void *list_tail(list_t *list);

/**
 * @brief `list_head` searches for the HEAD.
 * @param list a list to search
 * @return the pointer to the HEAD
 */
void *list_head(list_t *list);

/**
 * @brief `list_peek` searches for the `index`th item of `list`.
 * @param list a list to search an item from
 * @param index range: 0 <= index < list.length
 * @return the pointer to the item
 */
void *list_peek(list_t *list, int index);

/**
 * @brief `list_size` returns the size of this `list`.
 * @param list a list whose size is in doubt
 * @return the size of this list
 */
size_t list_size(list_t *list);

/**
 * @brief `list_empty` returns `true` if `list` has no item in it; otherwise, `false`.
 * @param list a list to check for its emptiness
 * @return whether this `list` is empty or not
 */
bool list_empty(list_t *list);

/**
 * @brief `list_full` returns `true` if `list` reaches a limit which it can accommodate; otherwise, `false`.
 * @param list a list to check for its fullness
 * @return whether this `list` is full or not
 */
bool list_full(list_t *list);

/**
 * @brief `list_indexof` searches for the item, inside the `list`, which points
 *    to the same object with `data`. If found, it returns the index of the item.
 * @param list a list to examine
 * @param data a pointer to be used when examining
 * @note `list` must not be empty and `data` must be somewhere in the `list`.
 *    Otherwise, the return value of this function is meaningless.
 */
size_t list_indexof(list_t *list, void *data);

/**
 * @brief `list_foreach` invokes `callback` on every element of `list`.
 * @param list a list to perform `list_foreach`
 * @param callback a function to be called; this function is passed two
 *    parameters. (1) `data` is the data of an element. (2) `index` is
 *    the index of that element.
 */
void list_foreach(list_t *list, void (*callback)(void *data, size_t index));

#endif
