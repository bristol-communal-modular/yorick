#include <stdbool.h>
#include <stdint.h>

#include "controller.h"

void controller_init(Controller *c) {
  c->current = 0;
  c->locked = false;
  c->previous = 0;
  for (uint8_t i = 0; i < CONTROL_COUNT; i++) {
    c->values[i] = 0;
  }
  c->freq = 0;
  for (uint8_t i = 0; i < DEBOUNCE_SAMPLES; i++) {
    c->freq_debounce_samples[i] = 0;
  }
  c->freq_debounce_count = 0;
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
      c->values[CONTROL_TUNING] = (value >> 3);
      break;
    case CONTROL_OSC_WAVE:
      c->values[control] = (value >> 8);
      break;
    case CONTROL_LFO_WAVE:
      c->values[control] = (value >> 8);
      break;
    case CONTROL_LFO_RATE:
      c->values[CONTROL_LFO_RATE] = (value >> 4) + 1;
      break;
    case CONTROL_ENVELOPE_ATTACK:
      c->values[control] = value + 1;
      break;
    case CONTROL_ENVELOPE_DECAY:
      c->values[control] = value + 1;
      break;
    default:
      break;
  }
}

uint16_t controller_get_control(Controller *c, ControlType control) {
  if (control < 0 || control >= CONTROL_COUNT) { return 0; }
  return c->values[control];
}

void controller_set_freq(Controller *c, uint16_t value) {
  // make sure input isn't zero
  if (value < UNLOCK_THRESH) {
    return;
  }
  uint8_t scaled_value = value >> 6;

  c->freq_debounce_count += 1;
  if (c->freq_debounce_count >= DEBOUNCE_SAMPLES) {
    c->freq_debounce_count = 0;
  }
  c->freq_debounce_samples[c->freq_debounce_count] = value;

  bool debounced = true;
  for (uint8_t i = 1; i < DEBOUNCE_SAMPLES; i++) {
    if (c->freq_debounce_samples[i] != c->freq_debounce_samples[i - 1]) {
      debounced = false;
      break;
    }
  }
  if (debounced) {
    c->freq = scaled_value;
  }
}

uint16_t controller_get_freq(Controller *c) {
  return c->freq;
}

