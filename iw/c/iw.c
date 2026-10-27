#include "iw.internals.h"

int main(int argc, const char *argv[]) {
   int main_ret = EXIT_SUCCESS;
   config_statcode_t cfg_ret;

   checkif_argc1(argc, optmds, optmds_len);

   init_config(&cfg);
   parse_argv(argc, argv, optmds, optmds_len, cfg.sourcefiles);

   cfg_ret = apply_config(&cfg);
   switch (cfg_ret) {
      case ConfigStatcodeNormalExit:
         goto cleanup;

      case ConfigStatcodeNoMsg:  /* fall-through */
      case ConfigStatcodeNoManpage:
         main_ret = EXIT_FAILURE;
         goto cleanup;

      default: ;
   }

   exec_wizard(&cfg);

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
      " - a helper program to make a message like an installation wizard to stdout\n"
      "\n"
      "USAGE\n"
      "\t" EXECNAME " (-m | --message) string\n"
      "\t" EXECNAME " (-h | --help)";

   if (argc == 1) {
      usage(table, table_len, intro);
      exit(EXIT_SUCCESS);  /* early exit */
   }
}

static void exec_wizard(config_t *cfg) {
   const char *msg = cfg->msg;

   switch (cfg->lang) {
      case LangoptEnumEn: exec_wizard_en(msg); break;
      case LangoptEnumKo: exec_wizard_ko(msg); break;
      default: ;  /* control never reaches default */
   }
}

static const char *get_iw_str(langopt_enum_t lang) {
   switch (lang) {
      case LangoptEnumEn : return "INSTALL WIZARD";
      case LangoptEnumKo : return "설치 마법사";
      default: return "unrecognized language";
   }
}

static void print_title(const char *msg, langopt_enum_t lang) {
   /* ### MSG IWSTR ### */
   const char *lang_str = get_iw_str(lang);
   printf("### %s %s ###\n\n", msg, lang_str);
}

static void exec_wizard_en(const char *msg) {
   print_title(msg, LangoptEnumEn);
   printf(
      "The Install Wizard will install %s in your system.\n"
      "\n",
      msg
   );
   printf(
      "Press what you would like to:\n"
      "(1) Install(i) - install %s\n"
      "(2) Cancel(c) - stop the install\n"
      "\n"
      "> i\n"
      "\n",
      msg
   );
   printf(
      "Installing %s...\n"
      "\n",
      msg
   );
   printf(
      "The Install Wizard has successfully installed %s.\n"
      "\n"
      "Your system has been modified. To complete installation,\n"
      "reboot the system at another time.\n"
      "\n",
      msg
   );
   printf(
      "Enter 'e' to exit the wizard or choose other actions:\n"
      "(1) Exit(e) - exit the wizard\n"
      "(2) Reboot(b) - reboot the system to complete the install\n"
      "(3) Reinstall(r) - make a clean reinstall of %s\n"
      "\n"
      "> e\n",
      msg
   );
}

static void exec_wizard_ko(const char *msg) {
   print_title(msg, LangoptEnumKo);
   printf(
      "설치 마법사가 %s(을)를 사용자의 컴퓨터에 설치하\n"
      "려고 합니다.\n"
      "\n",
      msg
   );
   printf(
      "설치 마법사를 통해 어떤 작업을 합니까?\n"
      "(1) 설치(i) - %s(을)를 설치합니다.\n"
      "(2) 취소(c) - 설치를 취소합니다.\n"
      "\n"
      "> i\n"
      "\n",
      msg
   );
   printf(
      "설치 중: %s...\n"
      "\n",
      msg
   );
   printf(
      "설치 마법사가 성공적으로 %s(을)를 사용자의 컴퓨\n"
      "터에 설치했습니다.\n"
      "\n"
      "사용자의 시스템이 변경되었습니다. 설치를 완료하\n"
      "려면 시스템을 나중에 재부팅하십시오.\n"
      "\n",
      msg
   );
   printf(
      "'e'를 입력해 설치 마법사를 종료하거나 다른 작업을 수행하십시오.\n"
      "(1) 종료(e) - 설치 마법사를 종료합니다.\n"
      "(2) 재부팅(b) - 시스템을 재부팅해 설치를 마무리합니다.\n"
      "(3) 재설치(r) - %s(을)를 다시 설치합니다.\n"
      "\n"
      "> e\n",
      msg
   );

}
