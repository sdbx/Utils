#ifndef REPLACE_INTERNALS_H
#define REPLACE_INTERNALS_H

/************
 * INCLUDES *
 ************/
#include <stdlib.h>
#
#include "utils.h"
#include "parse.h"
#include "fatal.h"
#include "strutils.h"
#include "wrappers.h"
#include "rtdbparse.h"
#include "array.adt.h"
#include "colorcodes.h"

/**************
 * PROTOTYPES *
 **************/
static array_t *parse_replace_string(const char *rs);
static void rthandle_user(symbol_t *sym, array_t *sectarr);
static void rthandle_else(symbol_t *sym, array_t *sectarr);
static void rthandle_common(symbol_t *sym, array_t *sectarr, int idx);
static void synerr_invalid(const char *rt);
static void synerr_empty(void);

/********************
 * STATIC VARIABLES *
 ********************/
static int pre_user;

#endif
