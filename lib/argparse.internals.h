#ifndef ARGPARSE_INTERNALS_H
#define ARGPARSE_INTERNALS_H

#include <setjmp.h>
#include <string.h>
#include <stdlib.h>

#include "fatal.h"
#include "common.h"
#include "argparse.h"
#include "wrappers.h"
#include "strutils.h"
#include "array.adt.h"
#include "colorcodes.h"

typedef struct {
   argtok_t tok;
   argerr_t err;
   void *ctx;
} errinfo_t;

typedef struct {
   int argc;
   const char **argv;
   const char *arg;
   array_t *toks;
   argtok_t *tok;
   size_t toks_len;
   size_t idx;
   bool eoe;
   size_t errcnt_max;
   optmd_t *table;
   size_t table_len;
   array_t *sourcefiles;
   argerr_t err;
   array_t *err_infos;
   jmp_buf env;
   size_t errcnt;
} argp_ctx_t;

typedef struct a {
   optmd_t *table;
   size_t table_len;
} errhandle_unrecognized_ctx_t;

static void validate_schemas(argp_ctx_t *actx);
static void tokenize_argv(argp_ctx_t *actx);
static void parse_argtoks(argp_ctx_t *actx);

static void handle_posarg(argp_ctx_t *actx);

static void populate_as_dashdash(argp_ctx_t *actx);
static void populate_as_longflag(argp_ctx_t *actx);
static void populate_as_shortflag(argp_ctx_t *actx);
static void populate_as_posarg(argp_ctx_t *actx);

static optmd_t *query_by_shortname(argp_ctx_t *actx, char flag);
static optmd_t *query_by_longname(argp_ctx_t *actx, const char *flag);

static argtok_t *gettok(argp_ctx_t *actx);
static void ungettok(argp_ctx_t *actx);

static bool preprocess_shortflag(argp_ctx_t *actx);
static bool preprocess_longflag(argp_ctx_t *actx);
static bool lookahead(argp_ctx_t *actx);

static array_destructor_t destruct_argtok;
static array_destructor_t destruct_errinfo;

static opt_errhandler_t errhandle_seen_multiple;
static opt_errhandler_t errhandle_unrecognized;
static opt_errhandler_t errhandle_out_of_tok;
static void regerr(argp_ctx_t *actx, argtok_t *tok);

static void report_errors(argp_ctx_t *actx);

static array_t *find_opt_candidates(
   argtok_t *tok,
   errhandle_unrecognized_ctx_t *ctx
);

static void showargtoks(argp_ctx_t *actx, bool terse);
static void showargtok(void *dat, size_t idx, void *ctx);
static void showargtok_terse(void *dat, size_t idx, void *ctx);
static const char *resolve_argtok_kind(argtok_t *tok);

static void list_options(argp_ctx_t *actx);

#endif
