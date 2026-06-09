#include "rtdbparse.h"
#include "rtdbparse.internals.h"

#include <setjmp.h>

static jmp_buf env;

extern array_t *rtdbparse(array_t *linestr) {
   array_t *table, *sectarr;
   char *sectname;

   ls = linestr;
   len = array_size(ls);
   idx = -1;
   table = array_create();

   if (!setjmp(env))
      goto parse;
   else
      goto escape;

   parse:
   /* backward compatibility for usr.dat */
   if (!seek_section()) {
      sectarr = add_mapper(table, "user");
      while (parse_value(sectarr));
   }

   for (;;) {
      sectname = parse_section();
      sectarr = rtdbquery(table, sectname);
      if (!sectarr)
         sectarr = add_mapper(table, sectname);
      while (parse_value(sectarr));
   }

   escape:
   return table;
}

extern array_t *rtdbquery(array_t *table, const char *sectname) {
   int i, table_len;
   mapper_t *mapper;

   table_len = array_size(table);
   for (i = 0; i < table_len; i++) {
      mapper = array_get(table, i);
      if (strcmp(mapper->sect, sectname))
         continue;
      return mapper->addr;
   }
   return NULL;
}

extern void rtdbcheck(array_t *db) {
   mapper_t *m;
   array_t *sectarr;
   char *sectname;
   size_t dbsiz, threshold, sectarr_siz;

   dbsiz = array_size(db);
   threshold = MIN_USER_COUNT;

   for (size_t i = 0; i < dbsiz; i++) {
      m = array_get(db, i);
      sectarr = m->addr;
      sectname = m->sect;
      sectarr_siz = array_size(sectarr);

      if (!strcmp(sectname, "user")) {
         if (sectarr_siz >= threshold)
            continue;
         VERR("at least %zu user entries required but only %zu",
            threshold, sectarr_siz
         );
      }
      else {
         if (sectarr_siz)
            continue;
         VERR("section %s has no entry", sectname);
      }
   }
}

extern void destroy_rtdb(array_t *rtdb) {
   int rtdbsiz;
   mapper_t *m;

   rtdbsiz = array_size(rtdb);
   for (int i = 0; i < rtdbsiz; i++) {
      m = array_get(rtdb, i);
      free(m->sect);
      array_destroy(m->addr);
   }

   array_destroy(rtdb);
}

static array_t *add_mapper(array_t *table, const char *sectname) {
   array_t *sectarr;
   char *buf;
   mapper_t mapper;

   buf = safe_malloc(strlen(sectname) + 1);
   strcpy(buf, sectname);

   sectarr = array_create();

   mapper.sect = buf;
   mapper.addr = sectarr;

   array_append(table, &mapper, sizeof mapper);

   return sectarr;
}

static int seek_section(void) {
   skip_empty_lines();
   find_frontpos();
   if (l->run[frontpos] != '[')
      return 0;
   return 1;
}

static char *parse_section(void) {
   find_rearpos();

   if (l->run[rearpos] != ']') {
      reason = "no section closer found";
      synerr();
   }
   frontpos++;
   if (frontpos == rearpos) {
      reason = "no section name found";
      synerr();
   }

   l->run[rearpos] = '\0';
   return l->run + frontpos;
}

static int parse_value(array_t *sectarr) {
   char *buf;
   size_t bufsiz;

   if (seek_section())
      return 0;

   /* frontpos has been set by seek_section() */
   bufsiz = l->len - frontpos;
   buf = safe_malloc(bufsiz);
   memcpy(buf, l->run + frontpos, bufsiz - 1);
   buf[bufsiz - 1] = '\0';  /* remove \n */

   array_append(sectarr, buf, bufsiz);
   free(buf);

   return 1;
}

static void find_frontpos(void) {
   /* a non-empty line assumed */
   frontpos = strspn(l->run, whitespaces);
}

static void find_rearpos(void) {
   /* frontpos must have been set before */
   rearpos = l->len - 1;
   while (frontpos < rearpos) {
      if (isspace(l->run[rearpos]))
         rearpos--;
      else break;
   }
}

static void next_line(void) {
   if (++idx == len) longjmp(env, 1);
   l = array_get(ls, idx);
}

static void skip_empty_lines(void) {
   size_t spn;

   for (;;) {
      next_line();
      spn = strspn(l->run, whitespaces);
      if (l->len == spn)
         continue;
      if (l->run[spn] == '#')
         continue;
      break;
   }
}

static void synerr(void) {
   safe_fprintf(stderr,
      Cbred "<syntax error>" Creset " %s\n"
      Cbwhite "note:" Creset " problematic since here\n"
      "%4zu|" Cbblue "%s" Creset "\n",
      reason,
      l->num, l->run
   );
   exit(EXIT_FAILURE);
}
