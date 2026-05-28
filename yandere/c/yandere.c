#include "yandere.types.h"

int main(void) {
   seed();
   event = Event_default;
   install_sighandlers();
   for (;;) dispatch();
   return 0;
}

static void install_sighandlers(void) {
   safe_signal(SIGINT , sighandle_sigint);
   safe_signal(SIGQUIT, sighandle_sigquit);
   safe_signal(SIGTERM, sighandle_sigterm);
}

static void dispatch(void) {
   switch (event) {
      case Event_default: evthandle_default(); break;
      case Event_sigint : evthandle_sigint (); break;
      case Event_sigquit: evthandle_sigquit(); break;
      case Event_sigterm: evthandle_sigterm(); break;
      default: ERR("control reaches default");
   }
}
