#include "strutils.h"
#include "strutils.internals.h"

/* Note: refer to strutils.h for the inline definition */
extern char lastch(const char *line);

extern bool match(char ch, const char *scanset) {
   while (*scanset)
      if (ch == *scanset++)
         return true;
   return false;
}

extern size_t match_str(
   const char *  restrict str,
   const char ** restrict arr,
   size_t arrlen
) {
   for (size_t i = 0; i < arrlen; i++)
      if (!strcmp(str, arr[i]))
         return i;
   return arrlen;
}

extern size_t match_str_case(
   const char *  restrict str,
   const char ** restrict arr,
   size_t arrlen
) {
   for (size_t i = 0; i < arrlen; i++)
      if (!strcasecmp(str, arr[i]))
         return i;
   return arrlen;
}

extern char *basename(const char *src) {
   /* There are four possible cases for src:
         (1) .../name
         (2) name
         (3) .../
         (4) \0   */

   size_t len;  /* the length of src */
   const char *dpos;  /* position of '/' */
   char *dest;  /* a buffer to hold the result */

   /* Finds the part after the final slash, if any */
   len = strlen(src);
   dpos = strrchr(src, '/');
   dpos = (dpos != NULL) ? dpos + 1 : src;

   /* case 3 & 4 */
   if (*dpos == '\0')
      return NULL;

   /* case 1 & 2 */
   dest = safe_malloc(len + 1);  /* max size */
   strcpy(dest, dpos);

   return dest;
}

extern char *stem(const char *src) {
   char *ret, *cpos;

   ret = basename(src);
   if (!ret)
      return NULL;

   cpos = strrchr(ret, '.');
   if (cpos)
      *cpos = '\0';

   return ret;
}

extern char **split(
   const char * restrict src,
   const char * restrict mark,
   size_t *retsiz
) {
   /* EXAMPLE USAGE
         mark = "**"
         input  = "**a****b**"
         output = ["a", "b"] */

   /* Declares variables */
   const int marklen = strlen(mark);

   char **arr;  /* result array */
   size_t siz;     /* the current size of arr */
   size_t max;     /* the max size of arr */

   /* 0123456789012
      I like tacos!
             ^    ^
           ini    fin
      diff = fin - ini
         = 12 - 7
         = 5
         = strlen(tacos) */

   const char *ini, *fin;
   ptrdiff_t diff;

   char *buf;  /* a buffer to copy a substring */
   size_t rest;    /* a length of a string */

   /* Prepares an array of strings */
   siz = 0;
   max = 2;
   arr = safe_malloc(max * ESIZ(arr));

   /* Parsing */
   ini = src;
   do {
      /* Searches the location of the next mark */
      fin = strstr(ini, mark);

      /* No mark found? */
      if (!fin) {
         /* since there is no more mark, copies
            the rest of the string into buf */
         rest = strlen(ini);
         buf = safe_malloc(rest + 1);
         memcpy(buf, ini, rest + 1);  /* includes \0 */
      }
      else {
         /* Copies the substring into buf */
         diff = fin - ini;  /* equals the token length */
         buf = safe_malloc(diff + 1);  /* +1 for \0 */
         memcpy(buf, ini, diff);
         buf[diff] = '\0';

         /* Updates parsing states */
         ini = fin + marklen;  /* skips the mark found */
      }

      /* Stores the buffer into the array */
      if (siz == max) {
         arr = safe_realloc2x_arr(arr, &max, ESIZ(arr));
      }
      arr[siz++] = buf;
   } while (fin);

   *retsiz = siz;
   return arr;
}

extern void trim(char *src) {
   char *fin, *ini;
   size_t len;

   len = strlen(src);

   /* Searches for a non-whitespace character from the back */
   for (fin = src + len; fin > src; fin--)
      if (!isspace((unsigned char) fin[-1]))
         break;

   /* Removes the trailing whitespaces */
   *fin = '\0';

   if (fin == src)  /* when src consists of only whitespaces */
      return;

   /* Finds the first non-whitespace character */
   ini = src + strspn(src, whitespaces);

   /* Removes the preceding whitespaces by moving ini to src */
   len = strlen(ini);
   memmove(src, ini, len + 1);  /* +1 for \0 */
}

extern void translate(
   char * restrict src,
   const char * restrict from,
   const char * restrict to
) {
   char ch, *pos;
   ptrdiff_t idx;

   for (size_t i = 0; (ch = src[i]); i++) {
      pos = strchr(from, ch);
      if (!pos) continue;
      idx = pos - from;
      src[i] = to[idx];
   }
}

extern void normalize(char *src) {
   size_t sz = strlen(src);

   for (size_t i = 0; i < sz; i++) {
      if (isspace((unsigned char) src[i])) {
         src[i] = ' ';
      }
   }
}

extern char *make_destname(
   const char * restrict orig,
   const char * restrict ext
) {
   char *temp, *buf;
   size_t tempsiz;

   temp = stem(orig);
   if (!temp)
      ERR("filename is empty");
   tempsiz = strlen(temp);
   buf = safe_malloc(tempsiz + strlen(ext) + 1);
   strcpy(buf, temp);
   strcat(buf, ext);

   free(temp);

   return buf;
}

extern int strcasecmp(const char *s1, const char *s2) {
   int c1, c2;

   while (*s1 && *s2) {
      c1 = tolower((unsigned char) *s1);
      c2 = tolower((unsigned char) *s2);

      if (c1 != c2)
         return c1 - c2;

      s1++, s2++;
   }

   return tolower((unsigned char) *s1) - tolower((unsigned char) *s2);
}

extern char *tolower_str(const char *orig) {
   size_t bufsiz;
   char *dest;

   bufsiz = strlen(orig);
   dest = safe_malloc(bufsiz + 1);  /* +1 for \0 */

   for (size_t i = 0; i < bufsiz; i++) {
      dest[i] = tolower((unsigned char) orig[i]);
   }
   dest[bufsiz] = '\0';

   return dest;
}

extern char *strdup(const char *src) {
   char *buf;
   size_t siz;

   siz = strlen(src);
   buf = safe_malloc(siz + 1);  /* +1 for \0 */
   memcpy(buf, src, siz + 1);  /* \0 copied */

   return buf;
}

extern char *strndup(const char *src, size_t n) {
   size_t idx, len;
   char *buf;

   /* the loop finishes once idx == n or src[idx] == 0 */
   idx = 0;
   while (idx < n && src[idx] != '\0') {
      idx++;
   }

   len = idx;
   buf = safe_malloc(len + 1);  /* +1 for \0 */
   memcpy(buf, src, len);
   buf[len] = '\0';

   return buf;
}

extern bool is_rnum(const char *rnum) {
   size_t i, k, nlen;
   place_t *p;
   const char *dat;

   for (i = 0; i < ps_len; i++) {
      p = ps + i;
      for (k = 0; k < 9; k++) {
         dat = (*p)[k];
         nlen = strlen(dat);
         if (strncmp(rnum, dat, nlen))
            continue;
         rnum += nlen;
         break;
      }
   }

   return !strlen(rnum);
}

extern unsigned int interpret_romnum(const char *romnum) {
   place_t *p;
   const char *s;
   unsigned int v, temp, len;

   v = 0;
   for (size_t i = 0; i < ps_len; i++) {
      p = ps + i;
      for (size_t j = 0; j < 9; j++) {
         s = (*p)[j];
         len = strlen(s);
         if (strncmp(s, romnum, len))
            continue;
         temp = 9 - j;
         switch (i) {
            case 0: temp *= 100; break;
            case 1: temp *=  10; break;
            default: ;
         }
         v += temp;
         romnum += len;
         break;
      }
   }

   return v;
}

/* C port of https://en.wikipedia.org/wiki/Levenshtein_distance#Computation */
extern int levenshtein_distance(const char *s, const char *t) {
   size_t sl, tl, subst_cost, ret;
   size_t *d;

   sl = strlen(s);  /* source length */
   tl = strlen(t);  /* target length */

   if (sl == 0) return tl;
   if (tl == 0) return sl;

   /* Note: for convinience, I put an arbitrary limit, on the length
      of s and t, which is 255 */
   static const size_t size_limit = 255;

   if (sl == size_limit || tl == size_limit) {
      return -1;
   }

   /* LAYOUT OF d[sl + 1][tl + 1]
         Note: +1 to represent an empty state

      # 0 t a r g e t
      0 - - - - - - -
      s - - - - - - -
      r - - - - - - -
      c - - - - - - -

      sl = the number of rows
      tl = the number of columns */

   d = safe_calloc((sl + 1) * (tl + 1), sizeof (size_t));

   #define D(r, c) d[(r) * ((tl) + 1) + (c)]

   for (size_t r = 0; r <= sl; r++) {
      D(r, 0) = r;
   }

   for (size_t c = 0; c <= tl; c++) {
      D(0, c) = c;
   }

   for (size_t c = 1; c <= tl; c++) {
      for (size_t r = 1; r <= sl; r++) {
         subst_cost = (s[r - 1] == t[c - 1]) ? 0 : 1;
         D(r,c) = min3(
            D(r - 1, c) + 1,  /* deletion of s[r] */
            D(r, c - 1) + 1,   /* insertion of t[c] */
            D(r - 1, c - 1) + subst_cost  /* match or substitution */
         );
      }
   }

   ret = D(sl, tl);
   free(d);
   #undef D
   return ret;  /* must representable as int due to size_limit */
}

extern find_alikes_ret_t find_alikes(
   const char *src,
   const void **targets,
   size_t targets_len,
   size_t target_siz
) {
   find_alikes_ret_t ret = {
      .alikes = NULL,
      .nmemb = 0
   };

   if (targets_len == 0) {
      goto cleanup;
   }

   size_t *dists = safe_malloc(targets_len * sizeof (size_t));

   /* Note. targets should be like:
         accept_set[] = { { "spl01", ... }, { "cor27", ... } }
         optmds[] = { "help", ... }, { "version", ... } }

      What GETSTR() does is to find each string in the array
      of unknown types. It assumes the string member is the first
      field in each struct! */

   /* Note: sizeof (char) = 1 */
   #define GETSTR(i) (* (char **) (((char *) targets) + (i) * (target_siz)))

   /* Evaluates distances */
   for (size_t i = 0; i < targets_len; i++) {
      dists[i] = levenshtein_distance(
         src, GETSTR(i)
      );
      if (dists[i] == NULL) {
         goto cleanup;
      }
   }

   size_t min = dists[0];

   /* Finds the smallest distance */
   for (size_t i = 1; i < targets_len; i++) {
      if (min > dists[i]) {
         min = dists[i];
      }
   }

   size_t results_len = 0;

   /* Counts the total length of the result array */
   for (size_t i = 0; i < targets_len; i++) {
      if (min == dists[i]) {
         results_len++;
      }
   }

   const char **results = safe_malloc(results_len * sizeof *results);

   /* Collects candidates which have the same distance with `min` */
   for (size_t i = 0, idx = 0; i < targets_len; i++) {
      if (min == dists[i]) {
         results[idx++] = GETSTR(i);
      }
   }

   ret.alikes = results;
   ret.nmemb = results_len;

cleanup:
   free(dists);
   #undef GETSTR

   return ret;
}
