#include "parse.h"
#include "parse.internals.h"

extern array_t *parse(array_t *tokstrs) {
   array_t *pts, *tokstr, *pt;
   int len;

   pts = array_create();
   len = array_size(tokstrs);

   for (int i = 0; i < len; i++) {
      tokstr = *((array_t **) array_get(tokstrs, i));
      pt = parse_tokstr(tokstr);
      array_append(pts, &pt, sizeof pt);
   }

   return pts;
}

extern void destroy_pts(array_t *pts) {
   int siz = array_size(pts);
   int ptsiz;
   array_t *pt;
   symbol_t *sym;

   for (int i = 0; i < siz; i++) {
      pt = *((array_t **) array_get(pts, i));
      ptsiz = array_size(pt);
      for (int k = 0; k < ptsiz; k++) {
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
   } while (state.pos != state.len);

   return state.pt;
}

static void tokhandle_chars(parse_state_t *state) {
   tokhandle_common(state, SymkindText);
}

static void tokhandle_delim(parse_state_t *state) {
   int prevpos;

   prevpos = state->pos;

   if (state->tok->run[0] != '$') {
      state->pos = prevpos;
      tokhandle_chars(state);
      return;
   }

   if (!nexttok(state))
      return;

   if (state->tok->kind != TokkindDelim) {
      state->pos = prevpos;
      tokhandle_chars(state);
      return;
   }

   if (state->tok->run[0] != '{') {
      state->pos = prevpos;
      tokhandle_chars(state);
      return;
   }

   if (!nexttok(state))
      return;

   if (state->tok->kind != TokkindChars) {
      state->pos = prevpos;
      tokhandle_chars(state);
      return;
   }

   if (!nexttok(state))
      return;

   if (state->tok->kind != TokkindDelim) {
      state->pos = prevpos;
      tokhandle_chars(state);
      return;
   }

   if (state->tok->run[0] != '}') {
      state->pos = prevpos;
      tokhandle_chars(state);
      return;
   }

   /*
    * In this context, we have ${xxx} but we are not
    * sure whether xxx is valid or not. If xxx is
    * invalid, we are to treat every token, i.e.
    * $, {, xxx, } as a character token.
    */
   state->tok = array_get(state->tokstr, state->pos - 1);
   if (strchr(state->tok->run, ' ')) {
      for (int i = prevpos; i <= state->pos; i++) {
         state->tok = array_get(state->tokstr, i);
         tokhandle_chars(state);
      }
      return;
   }

   tokhandle_common(state, SymkindRepl);
}

static void tokhandle_common(parse_state_t *state, symkind_t kind) {
   symbol_t sym;
   char *buf;

   buf = safe_malloc(state->tok->len + 1);
   strcpy(buf, state->tok->run);

   sym.kind = kind;
   sym.content = buf;

   array_append(state->pt, &sym, sizeof sym);

   state->pos++;
}

static int nexttok(parse_state_t *state) {
   state->pos++;
   if (state->pos == state->len)
      return 0;
   state->tok = array_get(state->tokstr, state->pos);
   return 1;
}
