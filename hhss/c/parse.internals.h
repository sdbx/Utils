#ifndef PARSE_INTERNALS_H
#define PARSE_INTERNALS_H

/************
 * INCLUDES *
 ************/
#include <stdlib.h>
#
#include "lex.h"
#include "debug.h"
#include "fatal.h"
#include "wrappers.h"
#include "array.adt.h"

/************
 * TYPEDEFS *
 ************/
typedef struct {
   array_t *pt;      // parse tree
   array_t *tokstr;  // token stream
   token_t *tok;     // token
   int len;  // token stream length
   int pos;  // current position in token stream
} parse_state_t;

/**************
 * PROTOTYPES *
 **************/
static array_t *parse_tokstr(array_t *tokstr);
static void tokhandle_chars(parse_state_t *state);
static void tokhandle_delim(parse_state_t *state);
static void tokhandle_common(parse_state_t *state, symkind_t kind);
static int nexttok(parse_state_t *state);

#endif
