#ifndef FATAL_H
#define FATAL_H

/**********
 * MACROS *
 **********/
#ifndef PROGNAME
#define PROGNAME "Utils"
#endif
#
#define ERR(errmsg) fatal(errmsg, __func__, __FILE__)
#define VERR(errmsg, ...) vfatal(errmsg, __func__, __FILE__, __VA_ARGS__)

/**************
 * PROTOTYPES *
 **************/
void fatal(
   const char * restrict errmsg,
   const char * restrict funcname,
   const char * restrict filename
);
void vfatal(
   const char * restrict errmsg,
   const char * restrict filename,
   const char * restrict funcname,
   ...
);

#endif
