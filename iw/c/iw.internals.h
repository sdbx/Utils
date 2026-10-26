#ifndef IW_INTERNALS_H
#define IW_INTERNALS_H

#include <stdio.h>
#include <stdlib.h>

#include "fatal.h"
#include "config.h"
#include "global.h"
#include "strutils.h"
#include "argparse.h"
#include "colorcodes.h"
#include "argp_drivers.h"

static void checkif_argc1(
   int argc,
   optmd_t table[],
   size_t table_len
);
static void exec_wizard(config_t *cfg);
static const char *get_iw_str(langopt_enum_t lang);
static void print_title(const char *msg, langopt_enum_t lang);
static void exec_wizard_en(const char *msg);
static void exec_wizard_ko(const char *msg);

#endif
