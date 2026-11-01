#ifndef UD_INTERNALS_H
#define UD_INTERNALS_H

#include <stdio.h>
#include <stdlib.h>

#include "config.h"
#include "global.h"
#include "common.h"
#include "colorcodes.h"
#include "argp_drivers.h"

typedef struct {
   const char *en;
   const char *ko;
} prefix_t;

static void checkif_argc1(
   int argc,
   optmd_t table[],
   size_t table_len
);

static void work(config_t *cfg);
static const char *get_prefix(langopt_enum_t lang, size_t idx);

static const prefix_t prefix[] = {
   {
      .en = "download",
      .ko = "다운로드"
   },
   {
      .en = "archive",
      .ko = "압축"
   },
   {
      .en = "build",
      .ko = "빌드"
   },
   {
      .en = "extract",
      .ko = "압축해제"
   },
   {
      .en = "remove",
      .ko = "삭제"
   }
};
static const size_t prefix_len = ARRLEN(prefix);

static const char *suffix[] = {
      "dll",
      "exe",
      "zip",
      "obj",
      "txt",
      "jpeg"
   };
static const size_t suffix_len = ARRLEN(suffix);

#endif
