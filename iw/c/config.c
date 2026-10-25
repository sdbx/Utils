#include "config.h"
#include "config.internals.h"

extern void init_config(config_t *cfg) {
   cfg->sourcefiles = array_create(NULL);
   cfg->msg = NULL;
   cfg->lang = LangoptEnumEn;
}

extern void destroy_config(config_t *cfg) {
   array_destroy(cfg->sourcefiles);
   free(cfg->msg);
}

extern config_statcode_t apply_config(config_t *cfg) {
   config_statcode_t ret = ConfigStatcodeOk;

   if ((ret = check_azusa(cfg)) != ConfigStatcodeOk)
      goto cleanup;
   if ((ret = check_help(cfg)) != ConfigStatcodeOk)
      goto cleanup;
   if ((ret = check_msg(cfg)) != ConfigStatcodeOk)
      goto cleanup;

cleanup:  /* nothing to clean up, just return */
   return ret;
}

static config_statcode_t check_azusa(config_t *cfg) {
   config_statcode_t ret = ConfigStatcodeOk;

   if (cfg->azusa_kawaii == false) {
      return ret;
   }

   /* Let's praise our kawaii Azusa chan */
   printf("UwU Azusa chan is so kawaii\n");
   ret = ConfigStatcodeNormalExit;

   return ret;
}

static config_statcode_t check_help(config_t *cfg) {
   config_statcode_t ret = ConfigStatcodeOk;

   if (cfg->help == false) {
      return ret;
   }

   return print_manpage();
}

static config_statcode_t check_msg(config_t *cfg) {
   config_statcode_t ret = ConfigStatcodeOk;

   if (cfg->msg != NULL) {
      return ret;
   }

   fprintf(stderr, ENPREFIX "no message given. Terminating.\n");
   ret = ConfigStatcodeNoMsg;

   return ret;
}

static config_statcode_t print_manpage(void) {
#if !defined(_WIN32) && (defined(__unix__) || defined(__unix))
   typedef enum { ShellNotAvail, ShellOk } system_rv_t;
   typedef enum { ManOk, ManFailed } man_exitstat_t;

   if (system(NULL) == ShellNotAvail) {
      return print_manual_fallback();
   }

   if (system("man " DIR_MANPAGE) == ManFailed) {
      return print_manual_fallback();
   }
   return ConfigStatcodeNormalExit;
#else
   return print_manual_fallback();
#endif
}

static config_statcode_t print_manual_fallback(void) {
   config_statcode_t ret = ConfigStatcodeNormalExit;
   FILE *fp;
   int ch;

   fp = fopen(DIR_MANPAGE_TXT, "r");
   if (fp == NULL) {
      fprintf(
         stderr,
         ENPREFIX "couldn't open the manpage from %s\n",
         DIR_MANPAGE_TXT
      );
      ret = ConfigStatcodeNoManpage;
      goto cleanup;
   }

   while ((ch = safe_fgetc(fp)) != EOF) {
      safe_fputc(stdout, ch);
   }

   safe_fclose(fp);

cleanup:
   return ret;
}
