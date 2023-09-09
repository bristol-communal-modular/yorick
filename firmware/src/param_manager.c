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

  pm->freq = 0;
  for (uint8_t i = 0; i < DEBOUNCE_SAMPLES; i++) {
    pm->freq_debounce_samples[i] = 0;
  }
  pm->freq_debounce_count = 0;
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

// This is kind of ugly
// it's compensating for the scaling of the keyboard voltages
// being a bit off. Should be fixable in hardware.
uint8_t param_manager_scale_freq(uint8_t value) {
  if (value < 4) {
    return 0;
  } else if (value < 8) {
    return 1;
  } else if (value < 12) {
    return 2;
  } else if (value < 14) {
    return 3;
  } else if (value < 18) {
    return 4;
  } else if (value < 21) {
    return 5;
  } else if (value < 23) {
    return 6;
  } else if (value < 27) {
    return 7;
  } else if (value < 31) {
    return 8;
  } else if (value < 35) {
    return 9;
  } else if (value < 39) {
    return 10;
  } else {
    return 11;
  }
}

bool param_manager_set_freq(ParamManager *pm, uint16_t value) {
  // make sure input isn't zero
  if (value < UNLOCK_THRESH) {
    return false;
  }
  value = value >> 4;

  pm->freq_debounce_count += 1;
  if (pm->freq_debounce_count >= DEBOUNCE_SAMPLES) {
    pm->freq_debounce_count = 0;
  }
  pm->freq_debounce_samples[pm->freq_debounce_count] = value;

  bool debounced = true;
  for (uint8_t i = 1; i < DEBOUNCE_SAMPLES; i++) {
    if (pm->freq_debounce_samples[i] != pm->freq_debounce_samples[i - 1]) {
      debounced = false;
      break;
    }
  }
  if (debounced) {
    pm->freq = param_manager_scale_freq(value);
  }
  return debounced;
}

ParamType param_manager_current(ParamManager *pm, uint8_t pot) {
  return param_map[pm->bank][pot];
}
