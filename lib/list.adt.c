#include "list.adt.h"
#include "list.adt.internals.h"

extern list_t *list_create(list_destructor_t *destruct) {
   list_t *list;

   list = safe_malloc(sizeof *list);

   list->head = NULL;
   list->tail = NULL;
   list->size = 0;
   list->destruct = destruct;

   return list;
}

extern void list_cleanup(list_t *list) {
   if (list->destruct != NULL) {
      list_foreach(list, list->destruct);
   }

   while (list_pop(list, NULL) == ListStatcodeOk)
      /* empty */ ;
}

extern void list_destroy(list_t *list) {
   list_cleanup(list);
   free(list);
}

extern void *list_push(list_t *list, void *item, size_t item_size) {
   /* BEFORE
      node --> NULL
      (tail)

      AFTER
      node <--> new_node --> NULL
                (tail) */
   node_t *new_node = make_node(item, item_size);

   if (list->size == 0) {
      list->tail = new_node;
      list->head = new_node;
      new_node->prev = NULL;
      new_node->next = NULL;
   }
   else {
      list->tail->next = new_node;
      new_node->prev = list->tail;
      new_node->next = NULL;
      list->tail = new_node;
   }
   list->size++;

   return new_node->data;
}

extern void *list_unshift(list_t *list, void *item, size_t item_size) {
   /* BEFORE
      NULL <-- node
               (head)

      AFTER
      NULL <-- new_node <--> node
               (head) */
   node_t *new_node = make_node(item, item_size);

   if (list->size == 0) {
      list->tail = new_node;
      list->head = new_node;
      new_node->prev = NULL;
      new_node->next = NULL;
   }
   else {
      list->head->prev = new_node;
      new_node->next = list->head;
      new_node->prev = NULL;
      list->head = new_node;
   }
   list->size++;

   return new_node->data;
}

extern void *list_insert(list_t *list, size_t index, void *item, size_t item_size) {
   /* BEFORE
      prev_node <--> next_node
      (index-1)      (index)

      AFTER
      prev_node <--> new_node <--> next_node
      (index-1)      (index)       (index+1) */
   node_t *prev_node, *new_node, *next_node;

   if (index == 0) {
      return list_unshift(list, item, item_size);
   }

   if (index == list->size) {
      return list_push(list, item, item_size);
   }

   prev_node = get_node(list, index - 1);
   new_node = make_node(item, item_size);
   next_node = prev_node->next;

   new_node->next = next_node;
   next_node->prev = new_node;
   new_node->prev = prev_node;
   prev_node->next = new_node;

   list->size++;

   return new_node->data;
}

extern list_statcode_t list_pop(list_t *list, void *dest) {
   /* BEFORE
      node <--> node --> NULL
               (tail)

      AFTER
      node --> NULL
      (tail) */
   node_t *current_tail = list->tail;

   if (!current_tail) {
      return ListStatcodeFailed;
   }

   if (dest != NULL) {
      memcpy(dest, current_tail->data, current_tail->data_size);
   }

   if (list->size == 1) {
      free(current_tail->data);
      free(current_tail);
      list->head = NULL;
      list->tail = NULL;
   }
   else {
      node_t *new_tail = current_tail->prev;

      new_tail->next = NULL;
      free(current_tail->data);
      free(current_tail);
      list->tail = new_tail;
   }
   list->size--;

   return ListStatcodeOk;
}

extern list_statcode_t list_shift(list_t *list, void *dest) {
   node_t *current_head = list->head;

   if (!current_head) {
      return ListStatcodeFailed;
   }

   if (dest != NULL) {
      memcpy(dest, current_head->data, current_head->data_size);
   }

   if (list->size == 1) {
      free(current_head->data);
      free(current_head);
      list->head = NULL;
      list->tail = NULL;
   }
   else {
      node_t *new_head = current_head->next;

      new_head->prev = NULL;
      free(current_head->data);
      free(current_head);
      list->head = new_head;
   }
   list->size--;

   return ListStatcodeOk;
}

extern list_statcode_t list_erase(list_t *list, size_t index, void *dest) {
   node_t *prev_node, *current_node, *next_node;

   if (index == 0) {
      return list_shift(list, dest);
   }

   if (index == (list->size - 1)) {
      return list_pop(list, dest);
   }

   current_node = get_node(list, index);
   prev_node = current_node->prev;
   next_node = current_node->next;

   if (dest != NULL) {
      memcpy(dest, current_node->data, current_node->data_size);
   }

   prev_node->next = next_node;
   next_node->prev = prev_node;
   free(current_node->data);
   free(current_node);

   list->size--;

   return ListStatcodeOk;
}

extern void *list_tail(list_t *list) {
   return list->tail->data;
}

extern void *list_head(list_t *list) {
   return list->head->data;
}

extern void *list_peek(list_t *list, int index) {
   return get_node(list, index)->data;
}

extern size_t list_size(list_t *list) {
   return list->size;
}

extern bool list_empty(list_t *list) {
   return list->size == 0 ? true : false;
}

extern bool list_full(list_t *list) {
   (void) list;
   /* Since this implementation utilizes a linked list,
      this list has no limit on the number of elements it
      can accommodate unless `malloc()` gets failed */
   return false;
}

// fixme: 좀더 범용적으로 만드려면 데이터가 동등한지를 검사하는 함수를
// 별도 타입으로 빼서 제공하도록 해야할듯
extern size_t list_indexof(list_t *list, void *data) {
   node_t *node = list->head;
   size_t i;

   for (i = 0; i < list->size; i++) {
      if (node->data == data) {
         break;
      }
   }

   return i;
}

// fixme: callback을 typedef로 빼기
extern void list_foreach(list_t *list, void (*callback)(void *data, size_t index)) {
   node_t *n;
   size_t idx;

   n = list->head;
   idx = 0;

   while (n != NULL) {
      callback(n->data, idx++);
      n = n->next;
   }
}

static node_t *make_node(void *item, size_t item_size) {
   node_t *n;
   void *dup;

   n = safe_malloc(sizeof *n);
   dup = safe_malloc(item_size);

   memcpy(dup, item, item_size);
   n->data = dup;
   n->data_size = item_size;

   return n;
}

static node_t *get_node(list_t *list, size_t index) {
   node_t *node;
   size_t count;

   if (list->head == NULL) {
      return NULL;
   }

   node = list->head;
   count = 0;

   while (count++ < index) {
      node = node->next;
   }

   return node;
}
