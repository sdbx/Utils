#include "argv1parse.h"
#include "argv1parse.internals.h"

extern int argv1parse(const char *argv1, int threshold) {
   char *i;
   int ret;

   errno = 0;
   ret = strtol(argv1, &i, 10);

   if (errno == ERANGE)
      ERR("count out of range");
   if (i == argv1)
      ERR("conversion not performed");
   if (ret < MIN_DATA_COUNT)
      VERR("count must be %d at least", MIN_DATA_COUNT);
   if (ret > threshold)
      VERR("count must be less than or equal to %d", threshold);

   return ret;
}
