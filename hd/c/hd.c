#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

int transition(int state, int input);

int main(void)
{
  int state = 0;
  int current_char;

  if ((current_char = getchar()) == EOF) {
    fprintf(stderr, "hd: EOF detected.");
    exit(EXIT_FAILURE);
  }
  else ungetc(current_char, stdin);

  while ((current_char = getchar()) != EOF) {
    state = transition(state, current_char);
    if (state < 0) exit(EXIT_FAILURE);
    if (state == 5) {
      printf("# ");
    } else if (state == 9) {
      printf(" #");
    }
    printf("%c", current_char);
  }
  return 0;
}

int transition(int state, int input) {
  switch (state) {
    case 0: {
      switch (input) {
        case '#': return 1;
        case '\r': return 4;
        case '\n': return 0;
        default: return 5;
      }
    }
    case 1: {
      switch (input) {
        case ' ': return 2;
        default: return 9;
      }
    }
    case 2: {
      switch (input) {
        case '\r': return 3;
        case '\n': return 0;
        default: return 2;
      }
    }
    case 3: {
      switch (input) {
        case '\n': return 0;
        default: return 2;
      }
    }
    case 4: {
      return 0;
    }
    case 5: {
      switch (input) {
        case '\r': return 6;
        case '\n': return 0;
        default: return 7;
      }
    }
    case 6: {
      switch (input) {
        case '\n': return 0;
        default: return 5;
      }
    }
    case 7: {
      switch (input) {
        case '\r': return 8;
        case '\n': return 0;
        default: return 7;
      }
    }
    case 8: {
      switch (input) {
        case '\n': return 0;
        default: return 7;
      }
    }
    case 9: {
      switch (input) {
        case '\r': return 8;
        case '\n': return 0;
        default: return 7;
      }
    }
  }
  fprintf(stderr, "hd: [WARN] unexpected case");
  return -1;
}
