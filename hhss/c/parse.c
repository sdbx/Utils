#include "parse.h"
#include "parse.internals.h"

extern array_t *parse(array_t *tokstrs) {
   array_t *pts, *tokstr, *pt;
   size_t len;

   pts = array_create();
   len = array_size(tokstrs);

   for (size_t i = 0; i < len; i++) {
      tokstr = *((array_t **) array_get(tokstrs, i));
      pt = parse_tokstr(tokstr);
      array_append(pts, &pt, sizeof pt);
   }

   return pts;
}

extern void destroy_pts(array_t *pts) {
   size_t siz = array_size(pts);
   size_t ptsiz;
   array_t *pt;
   symbol_t *sym;

   for (size_t i = 0; i < siz; i++) {
      pt = *((array_t **) array_get(pts, i));
      ptsiz = array_size(pt);
      for (size_t k = 0; k < ptsiz; k++) {
         sym = array_get(pt, k);
         free(sym->content);
      }
      array_destroy(pt);
   }
   array_destroy(pts);
}

static array_t *parse_tokstr(array_t *tokstr) {
   parse_state_t state;

   state.len = array_size(tokstr);
   state.pos = 0;
   state.pt = array_create();
   state.tokstr = tokstr;

   do {
      state.tok = array_get(tokstr, state.pos);
      switch (state.tok->kind) {
         case TokkindChars: tokhandle_chars(&state); break;
         case TokkindDelim: tokhandle_delim(&state); break;
         default: ERR("control reaches default");
      }
      state.pos++;
   } while (state.pos != state.len);

   return state.pt;
}

static void tokhandle_chars(parse_state_t *state) {
   store_token(state, SymkindText);
}

static void tokhandle_delim(parse_state_t *state) {
   /* Archive the original position for later rewind */
   size_t initpos = state->pos;

   goto lookahead;

   /* Whenever lookahead fails, control reaches here */
fallback:
   state->pos = initpos;
   state->tok = array_get(state->tokstr, state->pos);
   tokhandle_chars(state);
   return;  /* EARLY RETURN */

   /* Find ${x} pattern */
lookahead:
   if (state->tok->run[0] != '$')
      goto fallback;  /* this token is not $ */

   if (!nexttok(state))
      goto fallback;  /* $ was the last token */

   if (state->tok->kind != TokkindDelim)
      goto fallback;  /* this token is not a delimiter */

   if (state->tok->run[0] != '{')
      goto fallback;  /* this token is not { */

   if (!nexttok(state))
      goto fallback;  /* { was the last token */

   if (state->tok->kind != TokkindChars)
      goto fallback;  /* this token is not a plain token */

   if (strchr(state->tok->run, ' '))
      goto fallback;  /* this token does not have a valid replace string */

   if (!nexttok(state))
      goto fallback;  /* this token was the last token */

   if (state->tok->kind != TokkindDelim)
      goto fallback;  /* this token is not a delimiter */

   if (state->tok->run[0] != '}')
      goto fallback;  /* this token is not } */

   /* Finally, we have a valid ${x} */
   state->tok = array_get(state->tokstr, state->pos - 1);
   store_token(state, SymkindRepl);

   // Note that state->pos points to }. Thus, state->pos
   // is to be advanced correctly in the do-while loop.
}

static void store_token(parse_state_t *state, symkind_t kind) {
   symbol_t sym;
   char *buf;

   /* copy the whole string including \0 */
   buf = safe_malloc(state->tok->len + 1);
   memcpy(buf, state->tok->run, state->tok->len + 1);

   sym.kind = kind;
   sym.content = buf;

   array_append(state->pt, &sym, sizeof sym);
}

static int nexttok(parse_state_t *state) {
   state->pos++;
   if (state->pos == state->len)
      return 0;
   state->tok = array_get(state->tokstr, state->pos);
   return 1;
}
