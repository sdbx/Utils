#ifndef LEX_INTERNALS_H
#define LEX_INTERNALS_H

/************
 * INCLUDES *
 ************/
#include <stdlib.h>
#include <stdbool.h>
#
#include "debug.h"
#include "array.adt.h"
#include "wrappers.h"

/**************
 * PROTOTYPES *
 **************/
static array_t *lex_line(const char *line);
static void tokenize(void);
static void charcopy(void);

/********************
 * STATIC VARIABLES *
 ********************/
static array_t *tokstr;
static size_t bufsiz, pos;
static char ch, *buf;

#endif
