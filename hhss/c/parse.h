#ifndef PARSE_H
#define PARSE_H

/************
 * INCLUDES *
 ************/
#include "array.adt.h"

/************
 * TYPEDEFS *
 ************/
typedef enum {
   SymkindText,
   SymkindRepl
} symkind_t;

typedef struct {
   symkind_t kind;
   char *content;
} symbol_t;

/**************
 * PROTOTYPES *
 **************/
array_t *parse(array_t *tokstrs);
void destroy_pts(array_t *pts);

#endif
