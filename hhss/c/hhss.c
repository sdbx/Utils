#include "hhss.internals.h"

int main(int argc, char *argv[]) {
   validate(argc);
   prepare(argv);
   array_foreach(pts, iterate);
   cleanup();
   return 0;
}

static void validate(int argc) {
   if (argc != 2)
      ERR("usage: hhss <num>");
}

static void prepare(char *argv[]) {
   seed();

   dtdb_linestr = dbload(DATA_DB_PATH, OLD_DATA_DB_PATH);
   dtdb = dtdbparse(dtdb_linestr);
   dtdbcheck(dtdb);

   rtdb_linestr = dbload(RPTP_DB_PATH, OLD_RPTP_DB_PATH);
   rtdb = rtdbparse(rtdb_linestr);
   rtdbcheck(rtdb);

   datcnt = argv1parse(argv[1], array_size(dtdb));

   charstrs = prelex(dtdb, datcnt);
   tokstrs = lex(charstrs);
   pts = parse(tokstrs);
   replace_templates(pts, rtdb);
}

static void iterate(void *item, int idx) {
   (void) idx;
   array_foreach(*((array_t **) item), print_pt);
}

static void print_pt(void *item, int idx) {
   symbol_t *sym = item;
   (void) idx;
   safe_fputs(stdout, sym->content);
}

static void cleanup(void) {
   destroy_pts(pts);
   destroy_tokstrs(tokstrs);
   destroy_charstrs(charstrs);
   destroy_rtdb(rtdb);
   unloadfl(rtdb_linestr);
   destroy_dtdb(dtdb);
   unloadfl(dtdb_linestr);
}
