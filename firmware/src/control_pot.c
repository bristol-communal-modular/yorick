#include <stdbool.h>
#include <stdint.h>

#include "control_pot.h"

void control_pot_init(ControlPot *cp) {
  cp->locked = true;
  cp->value = 0;
}

void control_pot_update(ControlPot *cp, uint16_t new_value) {
  if (cp->locked) {
    // don't check max value as the highest new_value we get through
    // from ADCs is 10bit
    if (new_value < CONTROL_POT_UNLOCK_THRESH ||
        (new_value > (cp->value - CONTROL_POT_UNLOCK_THRESH) &&
        new_value < (cp->value + CONTROL_POT_UNLOCK_THRESH))
     ) {
      // is locked
      return;
    }
    cp->locked = false;
  }

  cp->value = new_value;
}

