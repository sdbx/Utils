#ifndef GLOBAL_H
#define GLOBAL_H

/************
 * INCLUDES *
 ************/
#include <signal.h>

/**********
 * MACROS *
 **********/
#define PROGNAME "yandere"
#define BUFMAX 128

/************
 * TYPEDEFS *
 ************/
typedef void sighandler_t(int sig);

typedef enum {
   Event_default,
   Event_sigint,
   Event_sigquit,
   Event_sigterm
} event_t;

/********************
 * EXTERN VARIABLES *
 ********************/
extern const char *msgs[];
extern volatile sig_atomic_t event;
extern char buf[];

#endif
