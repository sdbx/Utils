#include "evthandlers.h"

extern void evthandle_default(void) {
   noise(msgs[EventDefault]);
   printf("%s ", buf);
}

extern void evthandle_sigint(void) {
   printf("\n\n%s\n\n", msgs[EventSigint]);
   exit(EXIT_FAILURE); /* intended */
}

extern void evthandle_sigquit(void) {
   noise(msgs[EventSigquit]);
   printf("%s ", buf);
}

extern void evthandle_sigterm(void) {
   noise(msgs[EventSigterm]);
   printf("%s ", buf);
}
