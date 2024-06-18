#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

void raise_err(char *err_msg);
/* delimiters */
void print_excl_mark(void);
void print_ques_mark(void);
void print_excl_mark_u(void);
void print_ques_mark_u(void);
void print_heart(void);

int main(int argc, char **argv) {
   if (argc != 2)
      raise_err("nsy2: argument count not equal to 2.");

   void (*print_mark)(void);
   int mark_kind_len;

   mark_kind_len = strlen(argv[1]);

   if (mark_kind_len == 2) {
      if (strncmp(argv[1], "eu", 2) == 0)
         print_mark = print_excl_mark_u;
      else if (strncmp(argv[1], "qu", 2) == 0)
         print_mark = print_ques_mark_u;
      else
         raise_err("nsy2: an unknown argument.");
   }
   else if (mark_kind_len == 1) {
      switch (argv[1][0]) {
         case 'e': print_mark = print_excl_mark; break;
         case 'q': print_mark = print_ques_mark; break;
         case 'h': print_mark = print_heart; break;
         default:  raise_err("nsy2: an unknown argument.");
      }
   }
   else  raise_err("nsy2: an unknown argument.");

   bool whitespace_previously;
   int ch;

   if ((ch = getchar()) == EOF)
      raise_err("nsy2: EOF detected.");
   else if (!isspace(ch)) {
      print_mark();
      ungetc(ch, stdin);
   }

   whitespace_previously = false;
   while ((ch = getchar()) != EOF)
      if (isspace(ch)) {
         if (!whitespace_previously) {           
            whitespace_previously = true;
            print_mark();
         }
      }
      else {
         if (whitespace_previously)
            whitespace_previously = false;
         putchar(ch);
      }
   if (!whitespace_previously)
      print_mark();
   putchar('\n');

   return 0;
}

void raise_err(char *err_msg) {
   fputs(err_msg, stderr);
   putc('\n', stderr);
   exit(EXIT_FAILURE);
}

/* ! */
inline void print_excl_mark(void) {
   fputs("\xEF\xBC\x81", stdout);   /* 0xFF01 */
}

/* ? */
inline void print_ques_mark(void) {
   fputs("\xEF\xBC\x9F", stdout);   /* 0xFF1F */
}

/* !' */
inline void print_excl_mark_u(void) {
   fputs(" \xC2\xA1 ", stdout);    /* 0x00A1 */
}

/* ?' */
inline void print_ques_mark_u(void) {
   fputs(" \xC2\xBF ", stdout);    /* 0x00BF */
}

/* ♥ */
void print_heart(void) {
   fputs("\xE2\x99\xA5", stdout);    /* 0x2665 */
}