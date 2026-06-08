#ifndef HHSS_INTERNALS_H
#define HHSS_INTERNALS_H

/************
 * INCLUDES *
 ************/
#include "lex.h"
#include "debug.h"
#include "parse.h"
#include "utils.h"
#include "fatal.h"
#include "dbload.h"
#include "global.h"
#include "prelex.h"
#include "replace.h"
#include "wrappers.h"
#include "loadfile.h"
#include "rtdbparse.h"
#include "dtdbparse.h"
#include "argv1parse.h"

/**************
 * PROTOTYPES *
 **************/
static void validate(int argc);
static void prepare(char *argv[]);
static array_iterator_t iterate;
static array_iterator_t print_pt;
static void cleanup(void);

/********************
 * STATIC VARIABLES *
 ********************/
static array_t *dtdb, *dtdb_linestr;
static array_t *rtdb, *rtdb_linestr;
static array_t *charstrs, *tokstrs, *pts;
static int datcnt;

#endif
