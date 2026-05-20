#include <stdio.h>
#include <signal.h>

typedef void sighandler_t(int sig);

static inline void trigger_loop(const char *str);
static inline sighandler_t handle_sigint;
static inline sighandler_t handle_sigquit;
static inline sighandler_t handle_sigterm;

const char *msgs[] = {
   "이제 내가 싫어진거야? ",
   "도망가려고? ",
   "킥킥, 고작 그런 수로 벗어날 수 있을 거라 생각했어? ",
   "어디가? 설마 도망가는건 아니지? "
};

int main(void) {
   signal(SIGINT , handle_sigint);
   signal(SIGQUIT, handle_sigquit);
   signal(SIGTERM, handle_sigterm);
   trigger_loop(msgs[0]);
   return 0;
}

static inline void trigger_loop(const char *str) {
   for (;;) fputs(str, stdout);
}

static inline void handle_sigint(int sig) {
   (void) sig;
   trigger_loop(msgs[1]);
}

static inline void handle_sigquit(int sig) {
   (void) sig;
   trigger_loop(msgs[2]);
}

static inline void handle_sigterm(int sig) {
   (void) sig;
   trigger_loop(msgs[3]);
}
