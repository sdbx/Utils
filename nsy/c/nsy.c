/***
 * NOISY.C   by Cor (2024.05.25)
 * This program generates a sentence including an exclamation
 * /question mark between each characters.
 * 
 * Usage: ./nsy MARK_KIND < TEXT_FILE_NAME
 * where
 *    MARK_KIND = e | q   (exclamation or question)
 *              = eu | qu (upside down)
 *    TEXT_FILE_NAME = the file name to be input-redirected.
 * 
 * <Technical Details>
 * As far as I know, there are various kinds of exclamation
 * marks; for those interested, it would be a great idea to
 * refer to the Wikipedia article to see the list. Meanwhile,
 * I've chosen the U+FF01 character since this is a fullwidth
 * character and I think it would help make the sentence "noisier."
 * With a similar reason, U+FF1F for a question mark. However,
 * it seems that there are no fullwidth characters for upside
 * down ? and !, for readability, I have no choice but to put
 * a space character in front/behind of it.
 ***/

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

void raise_err(char *err_msg);
int inspect_char(int ch);
/* delimiters */
void print_excl_mark(void);
void print_ques_mark(void);
void print_excl_mark_u(void);
void print_ques_mark_u(void);
void print_period(void);
void print_ellipsis(void);
void print_plus(void);
void print_comma_twice(void);
void print_heart(void);

int main(int argc, char *argv[]) {
   if (argc != 2)
      raise_err("nsy: argument count not equal to 2.");

   void (*print_mark)(void);
   int mark_kind_len;

   mark_kind_len = strlen(argv[1]);

   if (mark_kind_len == 2) {
      if (strncmp(argv[1], "eu", 2) == 0)
         print_mark = print_excl_mark_u;
      else if (strncmp(argv[1], "qu", 2) == 0)
         print_mark = print_ques_mark_u;
      else if (strncmp(argv[1], "pl", 2) == 0)
         print_mark = print_plus;
      else if (strncmp(argv[1], "el", 2) == 0)
         print_mark = print_ellipsis;
      else
         raise_err("nsy: an unknown argument.");
   }
   else if (mark_kind_len == 1) {
      switch (argv[1][0]) {
         case 'e': print_mark = print_excl_mark; break;
         case 'q': print_mark = print_ques_mark; break;
         case 'p': print_mark = print_period; break;
         case 'c': print_mark = print_comma_twice; break;
         case 'h': print_mark = print_heart; break;
         default:  raise_err("nsy: an unknown argument.");
      }
   }
   else  raise_err("nsy: an unknown argument.");
   
   int ch;
   int left_multibyte_count = 0;
   
   /* Handling the edge case described in the spec. */
   if ((ch = getchar()) == EOF)
      raise_err("nsy: EOF detected.");
   else if (!isspace(ch)) {
      print_mark();
      ungetc(ch, stdin);
   }

   while ((ch = getchar()) != EOF) {
      if (isspace(ch))
         continue;
      
      if (left_multibyte_count == 0) {
         putchar(ch);
         left_multibyte_count = inspect_char(ch);
      }
      else {
         putchar(ch);
         left_multibyte_count--;
      }

      if (left_multibyte_count == 0)
         print_mark();
   }
   putchar('\n');
   
   return 0;
}

/* prints an error message to stderr. */
void raise_err(char *err_msg) {
   fputs(err_msg, stderr);
   putc('\n', stderr);
   exit(EXIT_FAILURE);
}

/* returns how many remaining bytes to be
   printed the character has. */
int inspect_char(int ch) {
   if (ch < 128)  /* 0xxxxxxx */
      return 0;
   
   int multibyte_len = 0;
   int push_amount = 7;
   int temp;
   
   /* 110xxxxx, 1110xxxx, 11110xxx */
   do {
      temp = ch >> push_amount;
      temp &= 1;
      if (temp == 1)
         multibyte_len++;
      else
         break;
   } while (--push_amount);

   return multibyte_len - 1;
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

/* . */
inline void print_period(void) {
   putchar('.');  /* 0x002E */
}

/* ... */
inline void print_ellipsis(void) {
   fputs("\xE2\x80\xA6", stdout);   /* 0x2026 */
}

/* + */
inline void print_plus(void) {
   putchar('+');  /* 0x002B */
}

/* ,, */
inline void print_comma_twice(void) {
   fputs(",,", stdout);    /* 0x002C */
}

/* ♥ */
inline void print_heart(void) {
   fputs("\xE2\x99\xA5", stdout);    /* 0x2665 */
}