#include "wrappers.h"

/***********
 * stdio.h *
 ***********/
extern void safe_fputs(FILE *stream, const char *line) {
   if (fputs(line, stream) == EOF)
      ERR("fputs error");
}

/************
 * stdarg.h *
 ************/
static const char *vfprintf_errmsg = "vfprintf error";

extern void safe_fprintf(FILE *stream, const char *format, ...) {
   va_list ap;
   int ret;

   va_start(ap, format);
   ret = vfprintf(stream, format, ap);
   if (ret < 0) ERR(vfprintf_errmsg);
   va_end(ap);
}

extern void safe_vfprintf(FILE *stream, const char *format, va_list *ap) {
   int ret;

   ret = vfprintf(stream, format, *ap);
   if (ret < 0) ERR(vfprintf_errmsg);
}

/************
 * signal.h *
 ************/
extern void safe_signal(int sig, sighandler_t handler) {
   /* intentionally ignore SIG_ERR */
   (void) signal(sig, handler);
}
