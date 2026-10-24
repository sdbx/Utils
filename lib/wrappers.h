#ifndef WRAPPERS_H
#define WRAPPERS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

/************
 * TYPEDEFS *
 ************/
typedef void sigprocessor_t(int sig);

/**
 * @brief `safe_malloc` is a wrapper for `malloc`.
 * @note exits in failure.
 */
void *safe_malloc(size_t siz);

/**
 * @brief `safe_realloc2x` is a wrapper for `realloc`.
 * @note exits in failure.
 */
inline void *safe_realloc2x(void *orig, size_t *siz);

/**
 *
 */
void *safe_realloc2x_arr(void *orig_arr, size_t *cnt, size_t esiz);

/**
 * @brief `safe_calloc` is a wrapper for `calloc`.
 * @note exits in failure.
 */
void *safe_calloc(size_t n, size_t siz);

/**
 * @brief `safe_fopen` is a wrapper for `fopen`.
 * @note exits in failure.
 */
FILE *safe_fopen(const char *filename, const char *mode);

/**
 * @brief `safe_fclose` is a wrapper for `fclose`.
 * @note exits in failure.
 */
void safe_fclose(FILE *fp);

void safe_remove(const char *filename);
void safe_rename(const char *old, const char *new);

int safe_fgetc(FILE *fp);

/**
 *
 */
void safe_fputc(FILE *stream, char ch);

/**
 * @brief `safe_fputs` is a wrapper for `fputs`.
 * @note exits in failure.
 */
void safe_fputs(FILE *stream, const char *line);

/**
 * @brief `safe_vfprintf` is a wrapper for `vfprintf`.
 * @note exits in failure.
 */
void safe_vfprintf(FILE *stream, const char *format, ...);

/**
 * @brief `safe_vprintf` is a wrapper for `vprintf`.
 * @note exits in failure.
 */
void safe_vprintf(const char *format, ...);

void safe_vprintf2(const char *format, va_list ap);

/**
 *
 */
void safe_fwrite(
   void *src,
   size_t siz,
   unsigned int cnt,
   FILE *dest
);

/**
 *
 */
void safe_fseek(FILE *fp, long offset, int whence);

/**
 *
 */
void safe_fgetpos(FILE *fp, fpos_t *pos);

/**
 *
 */
void safe_fsetpos(FILE *fp, fpos_t *pos);

void safe_fprintf(FILE *fp, const char *fmt, int n, ...);
void safe_fscanf(FILE *fp, const char *fmt, int n, ...);

void safe_signal(int sig, sigprocessor_t handler);

/*====================*
 | INLINE DEFINITIONS |
 *====================*/
inline void *safe_realloc2x(void *orig, size_t *siz) {
   return safe_realloc2x_arr(orig, siz, sizeof (char));
}

#endif
