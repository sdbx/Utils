/***
 * BUNTANG.C   by Cor (2024.05.24)
 * This program prints a long string having no white-space characters,
 * given a text file from stdin. Thus a would-be attention seeker may
 * find this program useful for his doing so-called 'buntang.'
 * 
 * Usage: ./btn N < TEXT_FILE_NAME
 * where
 *    N = the number of characters before a new-line character;
 *        its range: 1 ~ 32,
 *    TEXT_FILE_NAME = the file name to be input-redirected.
 ***/

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <errno.h>

#define MIN_CHAR_COUNT   1
#define MAX_CHAR_COUNT  32

void raise_err(char *err_msg);
int inspect_char(int ch);

int main(int argc, char *argv[]) {
   if (argc != 2)
      raise_err("btn: argument count not equal to 2.");

   int num_char_until_newline;
   char *indicator;

   errno = 0;
   num_char_until_newline = strtol(argv[1], &indicator, 10);
   if (errno == ERANGE)
      raise_err("btn: argv[1] out of range");
   if (indicator == argv[1])
      raise_err("btn: failed to process argv[1].");
   if (num_char_until_newline < MIN_CHAR_COUNT ||
         num_char_until_newline > MAX_CHAR_COUNT)
      raise_err("btn: argv[1] out of range.");

   int ch;

   if ((ch = getchar()) == EOF)
      raise_err("btn: EOF detected.");
   else
      ungetc(ch, stdin);

   int printed_char_count = 0;
   int left_multibyte_count = 0;

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

      if (left_multibyte_count == 0) {
         printed_char_count++;
         if (printed_char_count == num_char_until_newline) {
            putchar('\n');
            printed_char_count = 0;
         }
      }
   }
   if (printed_char_count != 0)
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