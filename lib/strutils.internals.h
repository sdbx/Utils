#ifndef STRUTILS_INTERNALS_H
#define STRUTILS_INTERNALS_H

#include <ctype.h>
#include <stddef.h>

#include "fatal.h"
#include "common.h"
#include "wrappers.h"

typedef const char *place_t[9];

static const char *whitespaces = " \n\r\t\a\b\v\f";

static place_t ps[] = {  /* The order is intended */
   { "CM", "DCCC", "DCC", "DC", "D", "CD", "CCC", "CC", "C" }, /* 100 */
   { "XC", "LXXX", "LXX", "LX", "L", "XL", "XXX", "XX", "X" }, /*  10 */
   { "IX", "VIII", "VII", "VI", "V", "IV", "III", "II", "I" }  /*   1 */
};
static const size_t ps_len = ARRLEN(ps);

#endif
