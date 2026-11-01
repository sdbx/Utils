#ifndef ARGP_DRIVERS_INTERNALS_H
#define ARGP_DRIVERS_INTERNALS_H

#include <errno.h>
#include <limits.h>

#include "common.h"
#include "config.h"
#include "wrappers.h"
#include "strutils.h"
#include "colorcodes.h"
#include "argp_drivers.h"

typedef struct {
   const void **targets;
   size_t targets_len;
   size_t target_siz;
} errhandle_unrecognized_value_ctx_t;

static opt_driver_t drive_help;
static opt_driver_t drive_msg;
static opt_driver_t drive_iter;
static opt_driver_t drive_lang;

static opt_errhandler_t errhandle_unrecognized_value;

/*==================*
 | EXTERN VARIABLES |
 *==================*/
optmd_t optmds[] = {
   {
      .sname = 'h',
      .lname = "help",
      .drive = drive_help,
      .reqv = false,
      .help = "prints the manual page",
      .unique = true
   },
   {
      .sname = 'm',
      .lname = "msg",
      .drive = drive_msg,
      .reqv = true,
      .help = "specify the message to print out",
      .unique = false
   },
   {
      .sname = 'i',
      .lname = "iterate",
      .drive = drive_iter,
      .reqv = true,
      .help = "specify the number of iteration",
      .unique = true
   },
   {
      .sname = '\0',
      .lname = "lang",
      .drive = drive_lang,
      .reqv = true,
      .help = "specify the language to speak",
      .unique = true
   }
};
const size_t optmds_len = ARRLEN(optmds);

#endif
