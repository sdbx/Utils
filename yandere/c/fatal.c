#include "fatal.h"

extern void fatal(const char *errmsg, const char *funcname) {
   safe_fprintf(
      stderr,
      PROGNAME ": fatal: %s in " Cemerald "%s" Creset "\n",
      errmsg, funcname
   );
   exit(EXIT_FAILURE);
}

extern void vfatal(const char *errmsg, const char *funcname, ...) {
   va_list ap;

   safe_fputs(stderr, PROGNAME ": fatal: ");

   va_start(ap, funcname);
   safe_vfprintf(stderr, errmsg, &ap);
   va_end(ap);

   safe_fprintf(
      stderr,
      " in " Cemerald "%s" Creset "\n",
      funcname
   );

   exit(EXIT_FAILURE);
}
