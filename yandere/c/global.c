#include "global.h"

const char *msgs[] = {
   [EventDefault] = "이제 내가 싫어진 것이다?",
   [EventSigint ] = "도망갈 수 있을 거라 생각하지 마는 것이다",
   [EventSigquit] = "발버둥치지 않는 것이다",
   [EventSigterm] = "설마 도망가려는 것이다?"
};

volatile sig_atomic_t event;

char buf[BUFMAX];
