#ifndef READLINE_H
#define READLINE_H

#include <stdio.h>

#define READLINE_UNIT   64

int readln(FILE *fp, char **line, size_t *len);

#endif
