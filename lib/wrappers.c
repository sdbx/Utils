#include "wrappers.h"
#include "wrappers.internals.h"

/************
 * stdlib.h *
 ************/
/* inline */
extern void *safe_realloc2x(void *orig, size_t *siz);

extern void *safe_malloc(size_t siz) {
   void *ret = malloc(siz);
   if (!ret) ERR("malloc error");
   return ret;
}

extern void *safe_realloc2x_arr(void *orig_arr, size_t *cnt, size_t esiz) {
   size_t new_cnt;
   void *ret;

   new_cnt = (*cnt) * 2;
   ret = realloc(orig_arr, new_cnt * esiz);

   if (!ret) {
      ERR("realloc error");
   }

   *cnt = new_cnt;
   return ret;
}

extern void *safe_calloc(size_t n, size_t siz) {
   void *ret = calloc(n, siz);
   if (!ret) ERR("calloc error");
   return ret;
}

/***********
 * stdio.h *
 ***********/
extern FILE *safe_fopen(const char *filename, const char *mode) {
   FILE *fp = fopen(filename, mode);
   if (!fp) VERR("unable to open the file: %s", filename);
   return fp;
}

extern void safe_fclose(FILE *fp) {
   if (fclose(fp) != EOF)
      return;
   ERR("unable to close a stream");
}

extern void safe_remove(const char *filename) {
   if (!remove(filename))
      return;
   VERR("unable to remove the file: %s", filename);
}

extern void safe_rename(const char *old, const char *new) {
   if (!rename(old, new))
      return;
   VERR("unable to rename %s to %s", old, new);
}

extern int safe_fgetc(FILE *fp) {
   int c = fgetc(fp);
   if (c == EOF && ferror(fp))
      ERR("fgetc error");
   return c;
}

extern void safe_fputc(FILE *stream, char ch) {
   if (fputc(ch, stream) == EOF)
      ERR("fputc error");
}

extern void safe_fputs(FILE *stream, const char *line) {
   if (fputs(line, stream) == EOF)
      ERR("fputs error");
}

extern void safe_vfprintf(FILE *stream, const char *format, ...) {
   va_list ap;
   int ret;

   va_start(ap, format);
   ret = vfprintf(stream, format, ap);
   if (ret < 0) exit(EXIT_FAILURE);
   va_end(ap);
}

extern void safe_vprintf(const char *format, ...) {
   va_list ap;
   int ret;

   va_start(ap, format);
   ret = vprintf(format, ap);
   if (ret < 0) exit(EXIT_FAILURE);
   va_end(ap);
}

extern void safe_vprintf2(const char *format, va_list ap) {
   int ret;

   ret = vprintf(format, ap);
   if (ret < 0) {
      exit(EXIT_FAILURE);
   }
}

extern void safe_fwrite(
   void *src,
   size_t siz,
   unsigned int cnt,
   FILE *dest
) {
   if (fwrite(src, siz, cnt, dest) < cnt)
      ERR("fwrite error");
}

extern void safe_fseek(FILE *fp, long offset, int whence) {
   if (fseek(fp, offset, whence))
      ERR("fseek error");
}

extern void safe_fgetpos(FILE *fp, fpos_t *pos) {
   if (fgetpos(fp, pos))
      ERR("fgetpos error");
}

extern void safe_fsetpos(FILE *fp, fpos_t *pos) {
   if (fsetpos(fp, pos))
      ERR("fsetpos error");
}

extern void safe_fprintf(FILE *fp, const char *fmt, int n, ...) {
   va_list ap;
   int ret;  /* vfprintf returns int */

   va_start(ap, n);
   ret = vfprintf(fp, fmt, ap);
   if (ret < n)
      ERR("vfprintf error");
   va_end(ap);
}

extern void safe_fscanf(FILE *fp, const char *fmt, int n, ...) {
   va_list ap;
   int ret;  /* vfscanf returns int */

   va_start(ap, n);
   ret = vfscanf(fp, fmt, ap);
   if (ret != n)
      ERR("vfscanf error");
   va_end(ap);
}

/************
 * signal.h *
 ************/
extern void safe_signal(int sig, sigprocessor_t handler) {
   if (signal(sig, handler) == SIG_ERR)
      ERR("signal error");
}
