#include "argv1parse.h"
#include "argv1parse.internals.h"

extern int argv1parse(const char *argv1, int threshold) {
   char *i;
   long ret;

   errno = 0;
   ret = strtol(argv1, &i, 10);

   if (errno == ERANGE || ret > INT_MAX || ret < INT_MIN)
      ERR("count out of range");
   if (i == argv1)
      ERR("conversion not performed");
   if (*i != '\0')
      ERR("invalid trailing characters in argv[1]");
   if (ret < MIN_DATA_COUNT)
      VERR("count must be %d at least", MIN_DATA_COUNT);
   if (ret > (long) threshold)
      VERR("count must be less than or equal to %d", threshold);

   return (int) ret;
}
