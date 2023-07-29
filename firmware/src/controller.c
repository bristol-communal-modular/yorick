#include <stdbool.h>
#include <stdint.h>

#include "controller.h"

void controller_init(Controller *c) {
  c->current = 0;
  c->locked = false;
  c->previous = 0;
  for (int i = 0; i < CONTROL_COUNT; i++) {
    c->values[i] = 0;
  }
}

void controller_lock(Controller *c, uint16_t previous) {
  c->locked = true;
  c->previous = previous;
}

void controller_next_control(Controller *c) {
  c->locked = true;
  c->current += 1;
  if (c->current >= CONTROL_COUNT) {
    c->current = 0;
  }
}

void controller_set_current(Controller *c, uint16_t value) {
  if (c->locked) {
    // don't check max value as the highest value we get through
    // from ADCs is 10bit
    if (value < UNLOCK_THRESH ||
        (value > (c->previous - UNLOCK_THRESH) &&
        value < (c->previous + UNLOCK_THRESH))
     ) {
      return;
    }
    c->locked = false;
  }
  c->previous = value;
  controller_set_control(c, c->current, value);
}

void controller_set_control(Controller *c, uint8_t control, uint16_t value) {
  switch(control) {
    case CONTROL_FM_FREQ:
      c->values[CONTROL_FM_FREQ] = value;
      break;
    case CONTROL_FM_DEPTH:
      c->values[CONTROL_FM_DEPTH] = value;
      break;
    case CONTROL_OCTAVE:
      // constrain octave to between 0 and 128
      c->values[CONTROL_OCTAVE] = (value >> 4);
      break;
  }
}

uint16_t controller_get_control(Controller *c, uint8_t control) {
  if (control < 0 || control >= CONTROL_COUNT) { return 0; }
  return c->values[control];
}
