#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

typedef void sighandler_t(int sig);
typedef const char *conststr_t;
typedef struct _yandere_t {
   conststr_t
      init, sigint, sigquit, sigterm;
} yandere_t;

static inline void trigger_loop(const char *str);
static inline sighandler_t handle_sigint;
static inline sighandler_t handle_sigquit;
static inline sighandler_t handle_sigterm;

static const yandere_t msgs = {
   .init = "이제 내가 싫어진거야?",
   .sigint = "\n\n도망갈 수 있을 거라 생각하지 마\n",
   .sigquit = "킥킥, 고작 그런 수로 벗어날 수 있을 거라 생각했어?",
   .sigterm = "어디가? 설마 도망가는건 아니지?"
};

int main(void) {
   (void) signal(SIGINT , handle_sigint);
   (void) signal(SIGQUIT, handle_sigquit);
   (void) signal(SIGTERM, handle_sigterm);
   trigger_loop(msgs.init);
   return 0;
}

static inline void trigger_loop(const char *str) {
   for (;;) printf("%s ", str);
}

static inline void handle_sigint(int sig) {
   (void) sig;
   puts(msgs.sigint);
   exit(EXIT_FAILURE);  /* intended */
}

static inline void handle_sigquit(int sig) {
   (void) sig;
   trigger_loop(msgs.sigquit);
}

static inline void handle_sigterm(int sig) {
   (void) sig;
   trigger_loop(msgs.sigterm);
}
