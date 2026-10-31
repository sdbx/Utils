#include "argp_drivers.h"
#include "argp_drivers.internals.h"

static bool drive_help(argtok_t *tok, argerr_t *err) {
   (void) tok, (void) err;
   cfg.help = true;
   return true;
}

static bool drive_msg(argtok_t *tok, argerr_t *err) {
   (void) err;
   /* +1 for \0 */
   array_append(cfg.msgs, tok->value, strlen(tok->value) + 1);
   return true;
}

static bool drive_iter(argtok_t *tok, argerr_t *err) {
   char *i;
   unsigned long ret;

   err->errhandle = argp_errhandle_default;

   if (tok->value[strspn(tok->value, " \t\n\r\v\f")] == '-') {
      err->reason = "iteration must not be negative";
      return false;
   }

   errno = 0;
   ret = strtoul(tok->value, &i, 10);

   if (errno == ERANGE) {
      err->reason = "out of range";
      return false;
   }
   if (i == tok->value) {
      err->reason = "conversion not performed";
      return false;
   }
   if (*i != '\0') {
      err->reason = "invalid trailing characters";
      return false;
   }
   if (ret > SIZE_MAX) {
      err->reason = "iteration out of range";
      return false;
   }

   cfg.iter = ret;
   return true;
}

static bool drive_lang(argtok_t *tok, argerr_t *err) {
   typedef struct {
      const char *lang;
      langopt_enum_t enum_val;
   } mapper_t;

   static const mapper_t accept_set[] = {
      { "en", LangoptEnumEn },
      { "ko", LangoptEnumKo }
   };
   static const size_t accept_set_len = ARRLEN(accept_set);

   const mapper_t *map;

   for (size_t i = 0; i < accept_set_len; i++) {
      map = &accept_set[i];
      if (!strcmp(tok->value, map->lang)) {
         cfg.lang = map->enum_val;
         return true;
      }
   }

   /* error path */
   static const errhandle_unrecognized_value_ctx_t ctx = {
      .targets = (const void **) &accept_set,
      .targets_len = accept_set_len,
      .target_siz = ESIZ(accept_set)
   };

   err->errhandle = errhandle_unrecognized_value;
   err->ctx = &ctx;
   err->ctx_siz = sizeof ctx;
   return false;
}

static void errhandle_unrecognized_value(argtok_t *tok, const char *reason, void *ctx) {
   find_alikes_ret_t ret;
   errhandle_unrecognized_value_ctx_t *ectx;

   (void) reason;
   ectx = ctx;

   ret = find_alikes(
      tok->value,
      ectx->targets,
      ectx->targets_len,
      ectx->target_siz
   );

   safe_vprintf(
      "%s: unrecognized value " Cbwhite "'%s'" Creset ".",
      tok->field, tok->value
   );
   safe_vprintf(" Do you mean ");
   if (ret.nmemb == 1) {
      safe_vprintf(Cbgreen "'%s'" Creset "?\n", ret.alikes[0]);
   }
   else {
      safe_vprintf("one of the following?\n\t" Cbgreen);
      for (size_t i = 0; i < ret.nmemb; i++) {
         safe_vprintf("%s  ", ret.alikes[i]);
      }
      safe_vprintf(Creset "\n");
   }

   free(ret.alikes);
}
