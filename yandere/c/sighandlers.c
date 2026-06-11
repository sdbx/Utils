#include "sighandlers.h"

extern void sighandle_sigint(int sig) {
   (void) sig;
   event = EventSigint;
}

extern void sighandle_sigquit(int sig) {
   (void) sig;
   event = EventSigquit;
}

extern void sighandle_sigterm(int sig) {
   (void) sig;
   event = EventSigterm;
}
