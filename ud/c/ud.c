#include "ud.internals.h"

int main(int argc, const char *argv[]) {
   int main_ret = EXIT_SUCCESS;
   config_statcode_t cfg_ret;

   checkif_argc1(argc, optmds, optmds_len);

   init_config(&cfg);
   parse_argv(argc, argv, optmds, optmds_len, cfg.srcs);

   cfg_ret = apply_config(&cfg);
   switch (cfg_ret) {
      case ConfigStatcodeOk: {
         break;
      }

      case ConfigStatcodeNormalExit: {
         goto cleanup;
      }

      case ConfigStatcodeErr:
         [[fallthrough]];

      case ConfigStatcodeNoMsg:
         [[fallthrough]];

      case ConfigStatcodeNoManpage: {
         main_ret = EXIT_FAILURE;
         goto cleanup;
      }

      default:
         ;  /* control never reaches default */
   }

   work(&cfg);

cleanup:
   destroy_config(&cfg);

   return main_ret;
}

static void checkif_argc1(
   int argc,
   optmd_t table[],
   size_t table_len
) {
   static const char *intro =
      "NAME\n"
      "\t" Cbwhite EXECNAME Creset
      " - a fake updater\n"
      "\n"
      "USAGE\n"
      "\t" EXECNAME " (-m | --message) string\n"
      "\t" EXECNAME " (-h | --help)";

   if (argc == 1) {
      usage(table, table_len, intro);
      exit(EXIT_SUCCESS);  /* early exit */
   }
}

static void work(config_t *cfg) {
   size_t pidx, sidx, midx;
   size_t msgs_len = array_size(cfg->msgs);

   for (size_t i = 0; i < cfg->iter; i++) {
      pidx = rand_range(0, prefix_len - 1);
      sidx = rand_range(0, suffix_len - 1);
      midx = rand_range(0, msgs_len - 1);
      printf(
         "%s: %s.%s\n",
         get_prefix(cfg->lang, pidx),
         (char *) array_peek(cfg->msgs, midx),
         suffix[sidx]
      );
   }
}

static const char *get_prefix(langopt_enum_t lang, size_t idx) {
   switch (lang) {
      case LangoptEnumEn: return prefix[idx].en;
      case LangoptEnumKo: return prefix[idx].ko;
      default: return "unknown lang";  /* control never reaches default */
   }
}
