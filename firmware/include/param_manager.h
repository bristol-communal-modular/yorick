#pragma once

#include <stdint.h>
#include <stdbool.h>

#define PARAM_BANKS 3
#define PARAM_POTS 2
#define PARAM_COUNT 6

typedef enum {
  PARAM_TUNING,
  PARAM_OSC_WAVE,
  PARAM_LFO_WAVE,
  PARAM_LFO_RATE,
  PARAM_ENVELOPE_ATTACK,
  PARAM_ENVELOPE_DECAY,
} ParamType;

typedef struct ParamManager {
  uint8_t bank;
} ParamManager;

void param_manager_init(ParamManager *pm);

void param_manager_next_bank(ParamManager *pm);

ParamType param_manager_current(ParamManager *pm, uint8_t pot);
