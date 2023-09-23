#include <stdbool.h>
#include <stdint.h>

#include "control_pot.h"

void control_pot_init(ControlPot *cp) {
  cp->locked = true;
  cp->changed = false;
  cp->value = 0;
  cp->lock_value = 0;

  for (uint8_t i = 0; i < CONTROL_POT_FILTER_SAMPLES; i++) {
    cp->filter_samples[i] = 0;
  }
  cp->filter_count = 0;
}

void control_pot_filter(ControlPot *cp, uint16_t new_value) {
  cp->filter_samples[cp->filter_count] = new_value;
  cp->filter_count += 1;
  if (cp->filter_count >= CONTROL_POT_FILTER_SAMPLES) {
    cp->filter_count = 0;
  }

  uint16_t filtered = 0;
  for (uint8_t i = 0; i < CONTROL_POT_FILTER_SAMPLES; i++) {
    filtered = filtered + cp->filter_samples[i];
  }
  cp->value = filtered >> 2;
}

void control_pot_update(ControlPot *cp, uint16_t new_value) {
  control_pot_filter(cp, new_value);

  if (cp->locked) {
    // don't check max value as the highest new_value we get through
    // from ADCs is 10bit
    if (new_value < CONTROL_POT_UNLOCK_THRESH ||
        (new_value > (cp->lock_value - CONTROL_POT_UNLOCK_THRESH) &&
        new_value < (cp->lock_value + CONTROL_POT_UNLOCK_THRESH))
     ) {
      // is locked
      return;
    }
    cp->locked = false;
  }
}

void control_pot_lock(ControlPot *cp) {
  cp->locked = true;
  cp->lock_value = cp->value;
}
