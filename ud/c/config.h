#ifndef CONFIG_H
#define CONFIG_H

#include <stdbool.h>

#include "array.adt.h"

typedef enum {
   LangoptEnumEn,
   LangoptEnumKo
} langopt_enum_t;

typedef struct config {
   array_t *srcs;
   bool help;
   array_t *msgs;
   size_t iter;
   langopt_enum_t lang;
} config_t;

typedef enum {
   ConfigStatcodeOk,
   ConfigStatcodeNormalExit,
   ConfigStatcodeErr,
   ConfigStatcodeNoMsg,
   ConfigStatcodeNoManpage
} config_statcode_t;

void init_config(config_t *cfg);
void destroy_config(config_t *cfg);
config_statcode_t apply_config(config_t *cfg);

extern config_t cfg;

#endif
