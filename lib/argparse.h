#ifndef ARGPARSE_H
#define ARGPARSE_H

#include <stddef.h>
#include <stdbool.h>

#include "array.adt.h"

typedef struct argtok argtok_t;
typedef struct argerr argerr_t;
typedef bool opt_driver_t(argtok_t *tok, argerr_t *err);
typedef void opt_errhandler_t(argtok_t *tok, const char *reason, void *ctx);

typedef struct {
   char sname;  /* short name */
   const char *lname;  /* long name */
   opt_driver_t *drive;
   bool reqv;
   const char *help;
   size_t seen;  /* works as a counter */
   bool unique;
} optmd_t;

typedef enum {
   ArgtokShortFlag,
   ArgtokLongFlag,
   ArgtokOptionEnd,
   ArgtokPosArg
} argtok_kind_t;

struct argtok {
   argtok_kind_t kind;
   char *field;
   char *value;
   optmd_t *md;
   bool has_eq;  /* only meaningful in the long-flag context */
};

struct argerr {
   opt_errhandler_t *errhandle;
   const char *reason;
   const void *ctx;
   size_t ctx_siz;
};

void parse_argv(
   int argc,
   const char *argv[],
   optmd_t table[],
   size_t table_len,
   array_t *sourcefiles
);

void usage(
   optmd_t table[],
   size_t table_len,
   const char *intro
);

opt_errhandler_t argp_errhandle_default;

#endif
