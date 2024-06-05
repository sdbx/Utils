#include <stdio.h>
#include <ctype.h>

int t(int s, int c);

int main(void)
{
  int s = 0;
  int c;
  while ((c = getchar()) != EOF) {
    s = t(s, c);
    if (s < 0) return s;
    if (s == 5) {
      printf("# ");
    } else if (s == 9) {
      printf(" #");
    }
    printf("%c", c);
  }
  return 0;
}

int t(int s, int c) {
  switch (s) {
    case 0: {
      switch (c) {
        case '#': return 1;
        case '\r': return 4;
        case '\n': return 0;
        default: return 5;
      }
    }
    case 1: {
      switch (c) {
        case ' ': return 2;
        default: return 9;
      }
    }
    case 2: {
      switch (c) {
        case '\r': return 3;
        case '\n': return 0;
        default: return 2;
      }
    }
    case 3: {
      switch (c) {
        case '\n': return 0;
        default: return 2;
      }
    }
    case 4: {
      return 0;
    }
    case 5: {
      switch (c) {
        case '\r': return 6;
        case '\n': return 0;
        default: return 7;
      }
    }
    case 6: {
      switch (c) {
        case '\n': return 0;
        default: return 5;
      }
    }
    case 7: {
      switch (c) {
        case '\r': return 8;
        case '\n': return 0;
        default: return 7;
      }
    }
    case 8: {
      switch (c) {
        case '\n': return 0;
        default: return 7;
      }
    }
    case 9: {
      switch (c) {
        case '\r': return 8;
        case '\n': return 0;
        default: return 7;
      }
    }
  }
  fprintf(stderr, "[WARN] unexpected case");
  return -1;
}
