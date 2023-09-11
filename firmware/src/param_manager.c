#include <stdbool.h>
#include <stdint.h>

#include "param_manager.h"

const ParamType param_map[PARAM_BANKS][PARAM_POTS] = {
  {PARAM_TUNING, PARAM_OSC_WAVE},
  {PARAM_LFO_WAVE, PARAM_LFO_RATE},
  {PARAM_ENVELOPE_ATTACK, PARAM_ENVELOPE_DECAY},
};

void param_manager_init(ParamManager *pm) {
  pm->bank = 0;
}

void param_manager_next_bank(ParamManager *pm) {
  pm->bank += 1;
  if (pm->bank >= PARAM_BANKS) {
    pm->bank = 0;
  }
}

ParamType param_manager_current(ParamManager *pm, uint8_t pot) {
  return param_map[pm->bank][pot];
}
