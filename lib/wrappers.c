#include "wrappers.h"
#include "wrappers.internals.h"

/***********
 * stdio.h *
 ***********/
extern FILE *safe_fopen(const char *filename, const char *mode) {
   FILE *fp = fopen(filename, mode);
   if (!fp) VERR("unable to open the file: %s", filename);
   return fp;
}

extern void safe_fclose(FILE *fp) {
   if (fclose(fp) != EOF) return;
   ERR("unable to close a stream");
}

extern void safe_fputs(FILE *stream, const char *line) {
   if (fputs(line, stream) == EOF)
      ERR("fputs error");
}

/************
 * stdlib.h *
 ************/
static const char *realloc_errmsg = "realloc error";

extern void *safe_malloc(size_t siz) {
   void *ret = malloc(siz);
   if (!ret) ERR("malloc error");
   return ret;
}

extern void *safe_calloc(size_t n, size_t esiz) {
   void *ret = calloc(n, esiz);
   if (!ret) ERR("calloc error");
   return ret;
}

extern void *safe_realloc2x(void *ptr, size_t *siz) {
   ptr = realloc(ptr, (*siz *= 2));
   if (!ptr) ERR(realloc_errmsg);
   return ptr;
}

extern void *safe_realloc2x_arr(void *arr, size_t *cnt, size_t esiz) {
   (*cnt) *= 2;
   arr = realloc(arr, (*cnt) * esiz);
   if (!arr) ERR(realloc_errmsg);
   return arr;
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
   if (signal(sig, handler) == SIG_ERR)
      ERR("signal error");
}
