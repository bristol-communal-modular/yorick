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

  for (uint8_t i = 0; i < PARAM_POTS; i++) {
    pm->pot_change_locked[i] = true;
    pm->previous_pot_value[i] = 0;
  }
}

void param_manager_lock(ParamManager *pm) {
  for (uint8_t i = 0; i < PARAM_POTS; i++) {
    pm->pot_change_locked[i] = true;
  }
}

void param_manager_next_bank(ParamManager *pm) {
  param_manager_lock(pm);
  pm->bank += 1;
  if (pm->bank >= PARAM_BANKS) {
    pm->bank = 0;
  }
}

bool param_manager_lock_check(ParamManager *pm, uint8_t pot, uint16_t value) {
  if (pm->pot_change_locked[pot]) {
    // don't check max value as the highest value we get through
    // from ADCs is 10bit
    if (value < UNLOCK_THRESH ||
        (value > (pm->previous_pot_value[pot] - UNLOCK_THRESH) &&
        value < (pm->previous_pot_value[pot] + UNLOCK_THRESH))
     ) {
      // is locked
      return true;
    }
    pm->pot_change_locked[pot] = false;
  }

  pm->previous_pot_value[pot] = value;
  // isn't locked
  return false;
}

ParamType param_manager_current(ParamManager *pm, uint8_t pot) {
  return param_map[pm->bank][pot];
}
