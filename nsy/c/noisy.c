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
void print_excl_mark(void);
void print_ques_mark(void);
void print_excl_mark_u(void);
void print_ques_mark_u(void);

int main(int argc, char *argv[]) {
   if (argc != 2)
      raise_err("nsy: argument count not equal to 2.");

   void (*print_mark)(void);

   if (strcmp(argv[1], "e") == 0)
      print_mark = print_excl_mark;
   else if (strcmp(argv[1], "q") == 0)
      print_mark = print_ques_mark;
   else if (strcmp(argv[1], "eu") == 0)
      print_mark = print_excl_mark_u;
   else if (strcmp(argv[1], "qu") == 0)
      print_mark = print_ques_mark_u;
   else
      raise_err("nsy: an unknown argument.");
   
   int ch;
   int left_multibyte_count = 0;

   print_mark();
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