#ifndef WRAPPERS_H
#define WRAPPERS_H

/************
 * INCLUDES *
 ************/
#include <stdio.h>
#include <stdarg.h>

/************
 * TYPEDEFS *
 ************/
typedef void sighandler_t(int sig);

/**************
 * PROTOTYPES *
 **************/
FILE *safe_fopen(const char *filename, const char *mode);
void safe_fclose(FILE *fp);
void safe_fputs(FILE *stream, const char *line);
void *safe_malloc(size_t siz);
void *safe_realloc2x(void *ptr, size_t *siz);
void *safe_realloc2x_arr(void *arr, size_t *cnt, size_t esiz);
void *safe_calloc(size_t n, size_t esiz);
void safe_fprintf(FILE *stream, const char *format, ...);
void safe_vfprintf(FILE *stream, const char *format, va_list *ap);
void safe_signal(int sig, sighandler_t handler);

#endif
