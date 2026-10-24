#ifndef CONFIG_INTERNALS_H
#define CONFIG_INTERNALS_H

#include <stdio.h>
#include <stdlib.h>

#include "global.h"
#include "config.h"
#include "wrappers.h"

static config_statcode_t check_azusa(config_t *cfg);
static config_statcode_t check_help(config_t *cfg);
static config_statcode_t check_msg(config_t *cfg);

static config_statcode_t print_manpage(void);

/*==================*
 | EXTERN VARIABLES |
 *==================*/
config_t cfg = {0};  /* Since config does not change, I made it external */

#endif
