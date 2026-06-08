#ifndef LEX_H
#define LEX_H

/************
 * INCLUDES *
 ************/
#include "array.adt.h"

/************
 * TYPEDEFS *
 ************/
typedef enum {
   Tokkind_chars,
   Tokkind_delim
} tokkind_t;

typedef struct {
   char *run;
   int len;
   tokkind_t kind;
} token_t;

/**************
 * PROTOTYPES *
 **************/
array_t *lex(array_t *charstrs);
void destroy_tokstrs(array_t *tokstrs);

#endif
