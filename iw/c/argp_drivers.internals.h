#ifndef ARGP_DRIVERS_INTERNALS_H
#define ARGP_DRIVERS_INTERNALS_H

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
static opt_driver_t drive_message;
static opt_driver_t drive_language;
static opt_driver_t drive_azusa_chan_kawaii;

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
      .lname = "message",
      .drive = drive_message,
      .reqv = true,
      .help = "specify the message to print out",
      .unique = true
   },
   {
      .sname = '\0',
      .lname = "lang",
      .drive = drive_language,
      .reqv = true,
      .help = "specify the language to speak",
      .unique = true
   },
   {
      .sname = '\0',
      .lname = "azusa-chan-kawaii",
      .drive = drive_azusa_chan_kawaii,
      .reqv = false,
      .help = "azusa chan is kawaii",
      .unique = false
   }
};
const size_t optmds_len = ARRLEN(optmds);

#endif
