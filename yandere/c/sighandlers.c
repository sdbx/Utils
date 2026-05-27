#include "sighandlers.h"

extern void sighandle_sigint(int sig) {
   (void) sig;
   event = Event_sigint;
   safe_signal(sig, sighandle_sigint);
}

extern void sighandle_sigquit(int sig) {
   (void) sig;
   event = Event_sigquit;
   safe_signal(sig, sighandle_sigquit);
}

extern void sighandle_sigterm(int sig) {
   (void) sig;
   event = Event_sigterm;
   safe_signal(sig, sighandle_sigterm);
}
