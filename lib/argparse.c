#include "argparse.h"
#include "argparse.internals.h"

/*
Improvement Ideas
   - argument taking optional value
      -O => uses its default value
      -O3 => uses 3 as its value
*/

extern void parse_argv(
   int argc,
   const char *argv[],
   optmd_t table[],
   size_t table_len,
   array_t *sourcefiles
) {
   argp_ctx_t *actx;

   if ((actx = malloc(sizeof *actx)) == NULL)
      goto OOM1;
   if ((actx->arena = arena_create()) == NULL)
      goto OOM2;
   if ((actx->toks = array_create(destruct_argtok)) == NULL)
      goto OOM3;
   if ((actx->err_infos = array_create(destruct_errinfo)) == NULL)
      goto OOM4;
   actx->argc = argc;
   actx->argv = argv;
   actx->idx = 0;
   actx->eoe = false;
   actx->err = (argerr_t) {
      .errhandle = argp_errhandle_default,
      .reason = "none",
      .ctx = NULL,
      .ctx_siz = 0
   };
   actx->errcnt = 0;
   actx->errcnt_max = 10;
   actx->table = table;
   actx->table_len = table_len;
   actx->sourcefiles = sourcefiles;

   if (setjmp(actx->env) != 0) {
      goto report_error;
   }

   validate_schemas(actx);
   tokenize_argv(actx);
   parse_argtoks(actx);

   // showargtoks(actx, true);

report_error:
   if (actx->errcnt > 0) {
      report_errors(actx);
      exit(EXIT_FAILURE);  // fixme: this is technically a resource leak
   }

cleanup:
   // since this wouldn't be invoked;
   // rewrite the logic and do not use setjmp / longjmp
   // the idea is: collect all resources even with a error path
   // the same thing applies to parse.c / lex.c
   array_destroy(actx->err_infos);
OOM4:
   array_destroy(actx->toks);
OOM3:
   arena_destroy(actx->arena);
OOM2:
   free(actx);
OOM1:
   return;
}

extern void usage(
   optmd_t table[],
   size_t table_len,
   const char *intro
) {
   safe_vprintf("%s\n\n", intro);

   safe_vprintf("OPTIONS\n");
   for (size_t i = 0; i < table_len; i++) {
      safe_vprintf("\t" Cyellow);
      if (table[i].sname != '\0' && table[i].lname == NULL)
         safe_vprintf("-%c" Creset, table[i].sname);
      else
      if (table[i].sname == '\0' && table[i].lname != NULL)
         safe_vprintf("--%s" Creset, table[i].lname);
      else
         safe_vprintf(
            "-%c" Creset ", " Cyellow "--%s" Creset,
            table[i].sname, table[i].lname);
      safe_vprintf("\n\t\t%s\n", table[i].help);
   }

   safe_vprintf(
      "\n"
      "tip: short flags can be specified in a"
         " row like " Ccyan "-xvf" Creset "\n"
      "tip: " Ccyan "--flag value" Creset
         " is the same as " Ccyan "--flag=value" Creset "\n"
      "tip: " Ccyan "-f value" Creset " is"
         " the same as " Ccyan "-fvalue" Creset "\n"
      "tip: use " Ccyan "--" Creset " to specify the end of option\n"
   );
}

static void validate_schemas(argp_ctx_t *actx) {
   size_t i, j;
   optmd_t *subject, *against;
   size_t errcnt;

   if (actx->table_len == 0) {
      goto fatal;
   }

   errcnt = 0;

   /* duplication check for short flags */
   for (i = 0; i < actx->table_len; i++) {
      subject = &actx->table[i];
      if (subject->sname == '\0')
         continue;
      for (j = i + 1; j < actx->table_len; j++) {
         against = &actx->table[j];
         if (against->sname == '\0')
            continue;
         if (subject->sname == against->sname) {
            safe_vprintf("md[%zu] has the same short name with md[%zu].\n", i, j);
            errcnt++;
         }
      }
   }

   /* checks if long flag is NULL */
   for (i = 0; i < actx->table_len; i++) {
      subject = &actx->table[i];
      if (subject->lname == NULL) {
         safe_vprintf("no long name is given for md[%zu].\n", i);
         errcnt++;
      }
   }

   /* duplication check for long flags */
   for (i = 0; i < actx->table_len; i++) {
      /* all flags must have its long name */
      subject = &actx->table[i];
      if (subject->lname == NULL) {
         /* the preceding validation doesn't mean that
            the flags do have their long names -- so
            this check is necessary */
         continue;
      }
      for (j = i + 1; j < actx->table_len; j++) {
         against = &actx->table[j];
         if (against->lname == NULL)
            continue;
         if (!strcmp(subject->lname, against->lname)) {
            safe_vprintf("md[%zu] has the same long name with md[%zu].\n", i, j);
            errcnt++;
         }
      }
   }

   if (errcnt == 0)
      return;

fatal:
   safe_fputs(stdout, "Currently the program has a problematic option table. Terminating.\n");
   exit(EXIT_FAILURE);  /* No choice but to kill it! */
}

static void tokenize_argv(argp_ctx_t *actx) {
   /* Note: begins from 1 to skip the program name */
   for (int i = 1; i < actx->argc; i++) {
      actx->arg = actx->argv[i];

      if (actx->eoe) {
         populate_as_posarg(actx);
         continue;
      }

      if (!strcmp(actx->arg, "--")) {
         actx->eoe = true;
         populate_as_dashdash(actx);
      }
      else
      if (!strncmp(actx->arg, "--", 2)) {
         populate_as_longflag(actx);
      }
      else
      if (actx->arg[0] == '-' && actx->arg[1] != '\0') {
         populate_as_shortflag(actx);
      }
      else {
         populate_as_posarg(actx);
      }
   }

   actx->toks_len = array_size(actx->toks);
}

static void parse_argtoks(argp_ctx_t *actx) {
   argtok_t *tok;
   bool preproc_result, parse_result;

   /* Note: toks has at least one element since argc > 1 supposed */
   /* Did not use gettok() on purpose; since it advances actx->idx,
      causing an unmatch between current token and its index */
   tok = actx->tok = array_peek(actx->toks, 0);

   while (actx->idx < actx->toks_len) {
      /* end of option? */
      if (tok->kind == ArgtokOptionEnd) {
         break;
      }

      /* positional argument? */
      if (tok->kind == ArgtokPosArg) {
         handle_posarg(actx);
         goto next;
      }

      /* unrecognized option? */
      if (tok->md == NULL) {
         actx->err.errhandle = errhandle_unrecognized;
         actx->err.ctx = & (errhandle_unrecognized_ctx_t) {
            .table = actx->table,
            .table_len = actx->table_len
         };
         actx->err.ctx_siz = sizeof (errhandle_unrecognized_ctx_t);
         regerr(actx, tok);
         goto next;
      }

      /* Since it's neither -- nor posarg, it's either a
         valid short or long flag, meaning tok->md must exist */

      /* specified multiple times? */
      if (tok->md->seen++ && tok->md->unique == true) {
         actx->err.errhandle = errhandle_seen_multiple;
         actx->err.ctx = tok->md;
         actx->err.ctx_siz = sizeof *tok->md;
         regerr(actx, tok);
         goto next;
      }

      switch (tok->kind) {
         /* to be accepted: "-j" | "-j4" | "-j 4" */
         case ArgtokShortFlag:
            preproc_result = preprocess_shortflag(actx);
         break;

         /* to be accepted: "--job" | "--job=4" | "--job 4" */
         case ArgtokLongFlag:
            preproc_result = preprocess_longflag (actx);
         break;

         /* control never reaches default */
         default: ;
      }
      if (preproc_result == false) {
         goto next;
      }

      /* Even if gettok() called once again in preprocess(),
         the value of this 'tok' would not be changed since
         it would have been assigned at the head of 'while' */
      parse_result = (*tok->md->drive)(tok, &actx->err);
      if (parse_result == false) {
         /* err has been filled by drive() */
         regerr(actx, tok);
      }

   next:
      if (actx->idx == actx->toks_len - 1) {
         return;  /* end of token */
      }
      tok = gettok(actx);
   }

   /* Collects the remanining positional arguments after -- as filenames */
   while (actx->idx < actx->toks_len - 1) {
      gettok(actx);
      handle_posarg(actx);
   }
}

static void handle_posarg(argp_ctx_t *actx) {
   array_append(actx->sourcefiles, actx->tok->field, strlen(actx->tok->field) + 1);
}

static void populate_as_dashdash(argp_ctx_t *actx) {
   argtok_t tok = {
      .kind = ArgtokOptionEnd,
      .field = NULL,
      .value = NULL,
      .md = NULL,
      .has_eq = false
   };

   array_append(actx->toks, &tok, sizeof tok);
}

static void populate_as_longflag(argp_ctx_t *actx) {
   argtok_t tok = {
      .kind = ArgtokLongFlag,
      .field = NULL,
      .value = NULL,
      .md = NULL,
      .has_eq = false
   };
   size_t eq_pos, arg_len;

   /*   01234567890123
      --target=haskell
        ^      ^
        field  value */

   actx->arg += 2;  /* skips -- */
   eq_pos = strcspn(actx->arg, "=");
   arg_len = strlen(actx->arg);

   tok.field = strndup(actx->arg, eq_pos);
   tok.md = query_by_longname(actx, tok.field);

   if (eq_pos < arg_len) {
      tok.has_eq = true;
      /* arg_len - eq_pos == 1 happens when no value given after = */
      if (arg_len - eq_pos > 1) {
         tok.value = strdup(&actx->arg[eq_pos + 1]);
      }
   }

   array_append(actx->toks, &tok, sizeof tok);
}

static void populate_as_shortflag(argp_ctx_t *actx) {
   argtok_t tok = {
      .kind = ArgtokShortFlag,
      .field = NULL,
      .value = NULL,
      .md = NULL,
      .has_eq = false
   };
   char flag;
   size_t arg_len;

   actx->arg++;  /* skips - */
   arg_len = strlen(actx->arg);

   for (
      /* empty */;
      (flag = actx->arg[0]) != '\0';
      actx->arg++, arg_len--
   ) {
      tok.field = strndup(actx->arg, 1);
      tok.md = query_by_shortname(actx, flag);

      if (tok.md != NULL) {
         if (tok.md->reqv && arg_len > 1) {
            /* Treats the rest of 'arg' as this short flag's value.
               For example:
                  -abcoa.out
                      ^~~~~~
               since -o is a value-taking option */
            tok.value = strdup(&actx->arg[1]);
         }
      }

      array_append(actx->toks, &tok, sizeof tok);

      /* if this flag were a value-taking option, the rest of 'arg'
         had been consumed as its value */
      if (tok.md != NULL && tok.md->reqv) {
         return;
      }
   }
}

static void populate_as_posarg(argp_ctx_t *actx) {
   argtok_t tok = {
      .kind = ArgtokPosArg,
      .field = strdup(actx->arg),
      .value = NULL,
      .md = NULL,
      .has_eq = false
   };

   array_append(actx->toks, &tok, sizeof tok);
}

static optmd_t *query_by_shortname(argp_ctx_t *actx, char flag) {
   optmd_t *md;

   for (size_t i = 0; i < actx->table_len; i++) {
      md = &actx->table[i];
      if (md->sname == '\0') {
         continue;
      }
      if (md->sname == flag) {
         return md;
      }
   }

   return NULL;
}

static optmd_t *query_by_longname(argp_ctx_t *actx, const char *flag) {
   optmd_t *md;

   for (size_t i = 0; i < actx->table_len; i++) {
      md = &actx->table[i];
      if (md->lname == NULL) {
         continue;
      }
      if (!strcmp(md->lname, flag)) {
         return md;
      }
   }

   return NULL;
}

static argtok_t *gettok(argp_ctx_t *actx) {
   if (actx->idx == actx->toks_len - 1) {
      actx->err.errhandle = errhandle_out_of_tok;
      regerr(actx, actx->tok);  /* dummy tok */
      longjmp(actx->env, 1);
   }

   actx->idx++;
   actx->tok = array_peek(actx->toks, actx->idx);

   return actx->tok;
}

static void ungettok(argp_ctx_t *actx) {
   actx->idx--;
   actx->tok = array_peek(actx->toks, actx->idx);
}

static bool preprocess_shortflag(argp_ctx_t *actx) {
   argtok_t *tok;

   tok = actx->tok;

   /* this option doesn't take a value */
   if (tok->md->reqv == false) {
      /* Note: tok->value must be NULL in this case
         due to the logic of populate_as_shortflag() */
      return true;
   }

   /* e.g. -j4 */
   if (tok->value != NULL) {
      return true;
   }

   /* e.g. -j 4 */
   if (lookahead(actx)) {
      return true;
   }

   /* error path! */
   ungettok(actx);
   actx->err.errhandle = argp_errhandle_default;
   actx->err.reason = "no positional argument follows";
   regerr(actx, actx->tok);
   return false;
}

static bool preprocess_longflag(argp_ctx_t *actx) {
   argtok_t *tok;

   tok = actx->tok;

   /* this option doesn't take a value */
   if (tok->md->reqv == false) {
      /* error path! */
      if (tok->value != NULL) {
         actx->err.errhandle = argp_errhandle_default;
         actx->err.reason = "doesn't require a value but given one";
         regerr(actx, tok);
         return false;
      }
      if (tok->has_eq) {
         actx->err.errhandle = argp_errhandle_default;
         actx->err.reason = "'=' is used with a non-value-taking option";
         regerr(actx, tok);
         return false;
      }
      /* a valid non-value-taking option */
      return true;
   }

   if (tok->has_eq) {
      /* error path! */
      if (tok->value == NULL) {
         actx->err.errhandle = argp_errhandle_default;
         actx->err.reason = "value not given after '='";
         regerr(actx, tok);
         return false; /* e.g. --job= */
      }
      else
         return true;  /* e.g. --job=4 */
   }

   /* e.g. --job 4 */
   if (lookahead(actx)) {
      return true;
   }

   /* error path! */
   ungettok(actx);
   actx->err.errhandle = argp_errhandle_default;
   actx->err.reason = "no positional argument follows";
   regerr(actx, actx->tok);
   return false;
}

static bool lookahead(argp_ctx_t *actx) {
   argtok_t *curr, *lookahead;

   curr = actx->tok;
   lookahead = gettok(actx);

   if (lookahead->kind == ArgtokPosArg) {
      /* tok->value is NULL, thus okay to overwrite */
      curr->value = strdup(lookahead->field);
      return true;
   }

   return false;
}

static void destruct_argtok(void *item, size_t idx) {
   argtok_t *tok;

   (void) idx;
   tok = item;
   free(tok->value);
   free(tok->field);
}

static void destruct_errinfo(void *item, size_t idx) {
   errinfo_t *ei;

   (void) idx;
   ei = item;
   free(ei->tok.field);
   free(ei->tok.value);
   free(ei->ctx);
}

extern void argp_errhandle_default(argtok_t *tok, const char *reason, void *ctx) {
   /* Note: for short flag and long flag, whose tok is fully filled */
   (void) ctx;
   safe_vprintf("%s: %s\n", tok->field, reason);
}

static void errhandle_seen_multiple(argtok_t *tok, const char *reason, void *ctx) {
   /* Note: for short flag and long flag, whose tok is fully filled */
   optmd_t *md;

   (void) reason;

   md = ctx;
   safe_vprintf(
      "%s: already specified %zu times\n",
      tok->field,
      md->seen
   );
}

static void errhandle_unrecognized(argtok_t *tok, const char *reason, void *ctx) {
   /* Note: only tok.field is available since this is not a valid option */
   static bool once = false;
   static const char *err_short = "unrecognized short flag";
   static const char *err_long  = "unrecognized long flag";

   errhandle_unrecognized_ctx_t *ectx;
   array_t *candidates;

   (void) reason;
   ectx = ctx;
   candidates = find_opt_candidates(tok, ectx);

   if (tok->kind == ArgtokShortFlag) {
      safe_vprintf("%s: %s\n", tok->field, err_short);
   }
   else /* long flag */ {
      safe_vprintf("%s: %s. Do you mean ", tok->field, err_long);
      /* at least one exists */
      if (array_size(candidates) == 1) {
         safe_vprintf(Cbgreen "--%s" Creset "?\n", array_peek(candidates, 0));
      }
      else {
         safe_vprintf("one of the following?\n\t" Cbgreen);
         for (size_t i = 0; i < array_size(candidates); i++) {
            safe_vprintf("%s  ", array_peek(candidates, i));
         }
         safe_vprintf(Creset "\n");
      }
      array_destroy(candidates);
   }

   if (!once) {
      safe_vprintf(
         "* Note: should this be a filename, specify '"
         Cbwhite "--" Creset "' first\n"
         "* Note: type "
         Cbmagenta "-h" Creset " or "
         Cbmagenta "--help" Creset " to see the manual page\n"
      );
      once = true;
   }
}

static void errhandle_out_of_tok(argtok_t *tok, const char *reason, void *ctx) {
   /* Note: can't use tok in this context */
   (void) tok, (void) reason, (void) ctx;
   safe_vprintf("argparse: ran out of command line arguments, having expected one\n");
}

static void regerr(argp_ctx_t *actx, argtok_t *tok) {
   errinfo_t ei = {
      .tok = *tok,
      .err = actx->err,  /* structs are copied */
      .ctx = NULL
   };

   /* Inits ei.tok */
   if (tok->field != NULL) ei.tok.field = strdup(tok->field);
   if (tok->value != NULL) ei.tok.value = strdup(tok->value);

   /* Inits ei.ctx */
   if (actx->err.ctx != NULL) {
      ei.ctx = safe_malloc(actx->err.ctx_siz);
      memcpy(ei.ctx, actx->err.ctx, actx->err.ctx_siz);
   }

   actx->errcnt++;
   array_append(actx->err_infos, &ei, sizeof ei);

   /* cleanup the error structure */
   actx->err.ctx = NULL;

   if (actx->errcnt == actx->errcnt_max) {
      longjmp(actx->env, 1);
   }
}

static void report_errors(argp_ctx_t *actx) {
   size_t len;
   errinfo_t *ei;

   len = array_size(actx->err_infos);

   for (size_t i = 0; i < len; i++) {
      ei = array_peek(actx->err_infos, i);
      ei->err.errhandle(&ei->tok, ei->err.reason, ei->ctx);
   }

   if (actx->errcnt < actx->errcnt_max) {
      return;
   }

   safe_vprintf("error count reached max."
      " Aborting argv parsing at idx = %zu.\n",
      actx->idx
   );
}

// fixme: find_alikes로 대체하기
// 그러려면 우선 옵션 배열 구조체에서 lname을 맨 위로 옮겨놔야 함 (optmds는 .lname=으로 정의되어 있으니까 수정할 필요 x)
static array_t *find_opt_candidates(
   argtok_t *tok,
   errhandle_unrecognized_ctx_t *ctx
) {
   size_t dists[ctx->table_len];  /* assumes table is not empty */

   /* Evaluates distances */
   for (size_t i = 0; i < ctx->table_len; i++) {
      dists[i] = levenshtein_distance(tok->field, ctx->table[i].lname);
   }

   size_t min = dists[0];

   /* Finds the smallest distance */
   for (size_t i = 1; i < ctx->table_len; i++) {
      if (min > dists[i]) {
         min = dists[i];
      }
   }

   array_t *results = array_create(NULL);

   /* Collects candidates which have the same distance with `min` */
   for (size_t i = 0; i < ctx->table_len; i++) {
      if (min == dists[i]) {
         array_append(
            results,
            ctx->table[i].lname,
            strlen(ctx->table[i].lname) + 1  /* +1 for \0 */
         );
      }
   }

   return results;
}

/*** DEBUG ***/

[[maybe_unused]]
static void showargtoks(argp_ctx_t *actx, bool terse) {
   if (terse) {
      printf(Ccyan Cul "idx\tkind\thas_eq\tseen\tfield\t\tvalue\n" Creset);
      array_foreach(actx->toks, showargtok_terse, NULL);
   }
   else
      array_foreach(actx->toks, showargtok, NULL);
}

static void showargtok(void *dat, size_t idx, void *ctx) {
   argtok_t *tok;

   (void) ctx;
   tok = dat;

   puts("==============================");
   printf("idx = %zu\n", idx);
   printf("kind = %s\n", resolve_argtok_kind(tok));
   printf("field = [%s]\n", tok->field ? tok->field : Cbblack "(empty)" Creset);
   printf("value = [%s]\n", tok->value ? tok->value : Cbblack "(empty)" Creset);
   printf("has_eq = %s\n", tok->has_eq ? "true" : "false");
   if (tok->md) {
      printf("\ncalled [-%c] or [--%s]\n", tok->md->sname, tok->md->lname);
      printf("seen count = %zu\n", tok->md->seen);
      printf("value-taking = %s\n", tok->md->reqv ? "true" : "false");
      printf("unique = %s\n", tok->md->unique ? "true" : "false");
   }
   putchar('\n');
}

static void showargtok_terse(void *dat, size_t idx, void *ctx) {
   argtok_t *tok;

   (void) ctx;
   tok = dat;

   printf("%zu\t%s\t%s\t",
      idx,
      resolve_argtok_kind(tok),
      tok->has_eq ? "true" : "false"
   );

   if (tok->md)
      printf("%zu", tok->md->seen);
   else
      printf(Cbblack "(n/a)" Creset);

   printf("\t%s\t\t", tok->field ? tok->field : Cbblack "(n/a)" Creset);
   printf("%s\n", tok->value ? tok->value : Cbblack "(empty)" Creset);
}

static const char *resolve_argtok_kind(argtok_t *tok) {
   switch (tok->kind) {
      case ArgtokShortFlag : return "short";
      case ArgtokLongFlag  : return "long";
      case ArgtokPosArg    : return "posarg";
      case ArgtokOptionEnd : return "--";
      default: ;  /* control never reaches here */
   }
   return NULL;
}

[[maybe_unused]]
static void list_options(argp_ctx_t *actx) {
   size_t i;

   puts("## SHORT FLAGS ##");
   for (i = 0; i < actx->table_len; i++) {
      if (actx->table[i].sname == '\0')
         continue;
      printf("\t%c\t(%s)\n", actx->table[i].sname, actx->table[i].lname);
   }

   puts("\n## LONG FLAGS ##");
   for (i = 0; i < actx->table_len; i++) {
      if (actx->table[i].lname == NULL)
         continue;
      printf("\t%s\n", actx->table[i].lname);
   }
}
