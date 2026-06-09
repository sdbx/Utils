#include "dbload.h"
#include "dbload.internals.h"

extern array_t *dbload(const char *new, const char *old) {
   if (canopen(new))
      return loadfile(new, NULL, NULL);
   return loadfile(old, NULL, NULL);
}

static int canopen(const char *filename) {
   FILE *fp;

   fp = fopen(filename, "r");
   if (!fp)
      return 0;

   (void) fclose(fp);

   return 1;
}
