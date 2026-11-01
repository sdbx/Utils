#include "config.h"
#include "config.internals.h"

extern void init_config(config_t *cfg) {
   cfg->srcs = array_create(NULL);
   cfg->msgs = array_create(NULL);
   cfg->iter = 8;
   cfg->lang = LangoptEnumEn;
}

extern void destroy_config(config_t *cfg) {
   array_destroy(cfg->srcs);
   array_destroy(cfg->msgs);
}

extern config_statcode_t apply_config(config_t *cfg) {
   config_statcode_t ret;

   if ((ret = check_help(cfg)) != ConfigStatcodeOk)
      goto cleanup;
   if ((ret = check_msg(cfg)) != ConfigStatcodeOk)
      goto cleanup;

cleanup:  /* nothing to clean up, just return */
   return ret;
}

static config_statcode_t check_help(config_t *cfg) {
   if (cfg->help == false) {
      return ConfigStatcodeOk;
   }

   return print_manpage();
}

static config_statcode_t check_msg(config_t *cfg) {
   if (array_size(cfg->msgs) > 0) {
      return ConfigStatcodeOk;
   }

   fprintf(stderr, ENPREFIX "--msg is a required flag. Terminating.\n");
   return ConfigStatcodeNoMsg;
}

static config_statcode_t print_manpage(void) {
#if !defined(_WIN32) && (defined(__unix__) || defined(__unix))
   if (!system(NULL)) {
      return print_manual_fallback();
   }

   if (system("man " DIR_MANPAGE)) {
      return print_manual_fallback();
   }

   return ConfigStatcodeNormalExit;
#else
   return print_manual_fallback();
#endif
}

static config_statcode_t print_manual_fallback(void) {
   FILE *fp;
   int ch;

   fp = fopen(DIR_MANPAGE_TXT, "r");
   if (fp == NULL) {
      fprintf(
         stderr,
         ENPREFIX "couldn't open the manpage from %s\n",
         DIR_MANPAGE_TXT
      );
      return ConfigStatcodeNoManpage;
   }

   while ((ch = fgetc(fp)) != EOF) {
      fputc(ch, stdout);
   }

   if (fclose(fp) == EOF) {
      return ConfigStatcodeErr;
   }

   return ConfigStatcodeNormalExit;
}
