#include "fatal.h"
#include "fatal.internals.h"

extern void fatal(
   const char * restrict errmsg,
   const char * restrict funcname,
   const char * restrict filename
) {
   (void) fprintf(
      stderr,
      PROGNAME ": fatal: %s [arose at " Cemerald "(*%s):%s" Creset "]\n",
      errmsg, funcname, filename
   );
   exit(EXIT_FAILURE);
}

extern void vfatal(
   const char * restrict errmsg,
   const char * restrict funcname,
   const char * restrict filename,
   ...
) {
   va_list ap;

   (void) fputs(PROGNAME ": fatal: ", stderr);

   va_start(ap, filename);
   (void) vfprintf(stderr, errmsg, ap);
   va_end(ap);

   (void) fprintf(
      stderr,
      " [arose at " Cemerald "(*%s):%s" Creset "]\n",
      funcname, filename
   );

   exit(EXIT_FAILURE);
}
