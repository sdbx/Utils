#ifndef RTDBPARSE_INTERNALS_H
#define RTDBPARSE_INTERNALS_H

/************
 * INCLUDES *
 ************/
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#
#include "fatal.h"
#include "global.h"
#include "wrappers.h"
#include "loadfile.h"
#include "array.adt.h"
#include "colorcodes.h"

/************
 * TYPEDEFS *
 ************/
typedef struct {
   char *sect;
   array_t *addr;
} mapper_t;

/**************
 * PROTOTYPES *
 **************/
static array_t *add_mapper(array_t *table, const char *sectname);
static int seek_section(void);
static char *parse_section(void);
static int parse_value(array_t *sectarr);
static void find_frontpos(void);
static void find_rearpos(void);
static void next_line(void);
static void skip_empty_lines(void);
static void synerr(void);

/********************
 * STATIC VARIABLES *
 ********************/
static const char *whitespaces = " \a\b\t\n\v\f\r";

static array_t *ls;
static line_t *l;
static size_t idx, len;
static size_t frontpos, rearpos;
static char *reason;

#endif
