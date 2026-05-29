#include "evthandlers.h"

extern void evthandle_default(void) {
   noise(msgs[Event_default]);
   printf("%s ", buf);
}

extern void evthandle_sigint(void) {
   printf("\n\n%s\n\n", msgs[Event_sigint]);
   exit(EXIT_FAILURE); /* intended */
}

extern void evthandle_sigquit(void) {
   noise(msgs[Event_sigquit]);
   printf("%s ", buf);
}

extern void evthandle_sigterm(void) {
   noise(msgs[Event_sigterm]);
   printf("%s ", buf);
}
