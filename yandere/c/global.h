#ifndef GLOBAL_H
#define GLOBAL_H

/************
 * INCLUDES *
 ************/
#include <signal.h>

/**********
 * MACROS *
 **********/
#define BUFMAX 128

/************
 * TYPEDEFS *
 ************/
typedef enum {
   EventDefault,
   EventSigint,
   EventSigquit,
   EventSigterm
} event_t;

/********************
 * EXTERN VARIABLES *
 ********************/
extern const char *msgs[];
extern volatile sig_atomic_t event;
extern char buf[];

#endif
