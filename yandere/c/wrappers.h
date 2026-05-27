#ifndef WRAPPERS_H
#define WRAPPERS_H

/************
 * INCLUDES *
 ************/
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#
#include "fatal.h"
#include "global.h"

/**************
 * PROTOTYPES *
 **************/
void safe_putchar(char ch);
void safe_fputs(FILE *stream, const char *line);
void safe_vfprintf(FILE *stream, const char *format, ...);
void safe_signal(int sig, sighandler_t handler);

#endif
