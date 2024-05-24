/***
 * NOISY.C   by Cor (2024.05.25)
 * This program generates a sentence including an exclamation
 * mark between each characters.
 * 
 * Usage: ./nsy < TEXT_FILE_NAME
 * where
 *    TEXT_FILE_NAME = the file name to be input-redirected.
 * 
 * <Technical Details>
 * As far as I know, there are various kinds of exclamation
 * marks; for those interested, it would be a great idea to
 * refer to the Wikipedia article to see the list. Meanwhile,
 * I've chosen the U+FF01 character since this is a fullwidth
 * character and I think it would help make the sentence "noisier."
 ***/

#include <stdio.h>
#include <ctype.h>

int inspect_char(int ch);
void print_excl_mark(void);

int main(int argc, char *argv[]) {
   int ch;
   int left_multibyte_count = 0;

   print_excl_mark();
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
         print_excl_mark();
   }
   putchar('\n');
   
   return 0;
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
void print_excl_mark(void) {
   putchar('\xEF');  /* 0xFF01 */
   putchar('\xBC');
   putchar('\x81');
}