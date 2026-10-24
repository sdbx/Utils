#ifndef CONFIG_H
#define CONFIG_H

#include <stdbool.h>

#include "array.adt.h"

typedef enum {
   LangoptEnumEn,
   LangoptEnumKo
} langopt_enum_t;

typedef struct config {
   array_t *sourcefiles;
   bool help;
   char *msg;
   langopt_enum_t lang;
   bool azusa_kawaii;
} config_t;

typedef enum {
   ConfigStatcodeOk,
   ConfigStatcodeNormalExit,
   ConfigStatcodeNoMsg,
   ConfigStatcodeNoManpage
} config_statcode_t;

void init_config(config_t *cfg);
void destroy_config(config_t *cfg);
config_statcode_t apply_config(config_t *cfg);

extern config_t cfg;

#endif
