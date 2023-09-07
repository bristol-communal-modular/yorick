#pragma once

#include <stdint.h>
#include <stdbool.h>

#define UNLOCK_THRESH 50
#define DEBOUNCE_SAMPLES 6

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
  bool pot_change_locked[PARAM_POTS];
  uint16_t previous_pot_value[PARAM_POTS];

  uint8_t bank;

  uint8_t freq;
  uint8_t freq_debounce_samples[DEBOUNCE_SAMPLES];
  uint8_t freq_debounce_count;
} ParamManager;

void param_manager_init(ParamManager *pm);

void param_manager_lock(ParamManager *pm);

void param_manager_next_bank(ParamManager *pm);

bool param_manager_lock_check(ParamManager *pm, uint8_t pot, uint16_t value);

bool param_manager_set_freq(ParamManager *pm, uint16_t value);

#define param_manager_get_freq(pm) (pm)->freq

ParamType param_manager_current(ParamManager *pm, uint8_t pot);
