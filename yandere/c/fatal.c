#include "fatal.h"

extern void fatal(const char *errmsg, const char *funcname) {
   safe_vfprintf(
      stderr,
      PROGNAME ": fatal: %s in " Cemerald "%s" Creset "\n",
      errmsg, funcname
   );
   exit(EXIT_FAILURE);
}

extern void vfatal(const char *errmsg, const char *funcname, ...) {
   va_list ap;
   int ret;

   va_start(ap, funcname);
   safe_fputs(stderr, PROGNAME ": fatal: ");
   safe_vfprintf(stderr, errmsg, ap);
   va_end(ap);
   ret = fprintf(stderr, " in " Cemerald "%s" Creset "\n", funcname);
   if (ret < 0) ERR("fprintf error");
   exit(EXIT_FAILURE);
}
