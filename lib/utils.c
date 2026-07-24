#include "utils.h"
#include "utils.internals.h"

extern void seed(void) {
   srand(time(NULL));
}

/* Refer to https://c-faq.com/lib/randrange.html */
extern int rand_range(int min, int max) {
   if (min > max)
      VERR("min must be less than or equal to max,"
         " but given min=%d, max=%d", min, max);

   /* implicit integer promotion happens */
   /* INT_MAX - INT_MIN == UINT_MAX */
   if ((unsigned int) max - min > RAND_MAX)
      VERR("range too large to handle!"
         " max - min must be <= %d (RAND_MAX),"
         " but given min=%d, max=%d", min, max);

   if (min == max)
      return min;

   unsigned int
      nbucket,  /* number of buckets */
      bucket_siz,
      threshold,
      rv;

   /* like there are 5 numbers in [1,5] since 5 - 1 + 1 = 5,
      max - min + 1 means the count of the numbers in [min, max] */
   /* since max - min <= RAND_MAX <= INT_MAX, it's fine to add 1 */
   nbucket = 1U + max - min;
   /* since rand() returns [0,RAND_MAX], the number of total
      possible return values is RAND_MAX - 0 + 1 */
   /* specify 'U' in order to treat it as a unsigned int value */
   /* integer promotion also happens */
   bucket_siz = (RAND_MAX + 1U) / nbucket;
   threshold = bucket_siz * nbucket;

   do rv = rand();
   while (rv >= threshold);

   return min + (int) (rv / bucket_siz);

   /* EXAMPLE CASE
      Suppose RAND_MAX = 10, nbucket = 3.
      Since bucket_siz = (10 + 1) / 3 = 3 (fractional part discarded),
      threshold = 3 * 3 = 9.
      Since RAND_MAX is 10, rand() returns [0,10].
      If rand returns 9 or 10, re-roll.
      If rand returns 0 ~ 8, then
         rv = 0,1,2 => rv / 3 = 0
         rv = 3,4,5 => rv / 3 = 1
         rv = 6,7,8 => rv / 3 = 2
      Thus, all numbers in [min, max] have an equal
      possibility to appear. */
}

extern int mblen_(char ch) {
   unsigned char c;

   c = (unsigned char) ch;

   if (c < 0x80) return 1;             /* 0xxxxxxx */
   if ((c & 0xE0) == 0xC0) return 2;   /* 110xxxxx */
   if ((c & 0xF0) == 0xE0) return 3;   /* 1110xxxx */
   if ((c & 0xF8) == 0xF0) return 4;   /* 11110xxx */

   return 0;
}
