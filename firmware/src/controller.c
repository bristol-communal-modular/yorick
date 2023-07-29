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

void controller_set_control(Controller *c, ControlType control, uint16_t value) {
  switch(control) {
    case CONTROL_TUNING:
      // constrain octave to between 0 and 127
      c->values[CONTROL_TUNING] = (value >> 4);
      break;
    case CONTROL_FM_FREQ:
      c->values[CONTROL_FM_FREQ] = value;
      break;
    case CONTROL_FM_DEPTH:
      c->values[CONTROL_FM_DEPTH] = (value >> 2);
      break;
    case CONTROL_ATTACK:
      break;
    case CONTROL_DECAY:
      break;
    case CONTROL_MOD_OUTPUT:
      break;
    case CONTROL_LFO_SPEED:
      // constrain LFO speed to between 1 and 128
      c->values[CONTROL_FM_FREQ] = (value >> 4) + 1;
      break;
    default:
      break;
  }
}

uint16_t controller_get_control(Controller *c, ControlType control) {
  if (control < 0 || control >= CONTROL_COUNT) { return 0; }
  return c->values[control];
}
