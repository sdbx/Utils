#include "wrappers.h"

/***********
 * stdio.h *
 ***********/
extern void safe_putchar(char ch) {
   if (putchar(ch) == EOF)
      ERR("putchar error");
}

extern void safe_fputs(FILE *stream, const char *line) {
   if (fputs(line, stream) == EOF)
      ERR("fputs error");
}

/************
 * stdarg.h *
 ************/
extern void safe_vfprintf(FILE *stream, const char *format, ...) {
   va_list ap;
   int ret;

   va_start(ap, format);
   ret = vfprintf(stream, format, ap);
   if (ret < 0) ERR("vfprintf error");
   va_end(ap);
}

/************
 * signal.h *
 ************/
extern void safe_signal(int sig, sighandler_t handler) {
   /* intentionally ignore SIG_ERR */
   (void) signal(sig, handler);
}
