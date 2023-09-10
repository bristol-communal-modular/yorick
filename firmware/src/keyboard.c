#include "keyboard.h"

void keyboard_init(Keyboard *k) {
  k->current_state = KEYBOARD_UNSTABLE;
  k->previous_state = KEYBOARD_UNSTABLE;

  k->key = 0;
  for (uint8_t i = 0; i < KEYBOARD_DEBOUNCE_SAMPLES; i++) {
    k->key_debounce_samples[i] = 0;
  }
  k->key_debounce_count = 0;
}

// This is kind of ugly
// it's compensating for the scaling of the keyboard voltages
// being a bit off. Should be fixable in hardware.
uint8_t keyboard_scale_value(uint8_t value) {
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
  } else if (value < 38) {
    return 10;
  } else {
    return 11;
  }
}

void keyboard_update(Keyboard *k, uint16_t value) {
  k->previous_state = k->current_state;

  // make sure input isn't zero
  if (value < KEYBOARD_MIN_VALUE) {
    k->current_state = KEYBOARD_UNSTABLE;
    return;
  }
  value = value >> 4;

  k->key_debounce_count += 1;
  if (k->key_debounce_count >= KEYBOARD_DEBOUNCE_SAMPLES) {
    k->key_debounce_count = 0;
  }
  k->key_debounce_samples[k->key_debounce_count] = value;

  bool debounced = true;
  for (uint8_t i = 1; i < KEYBOARD_DEBOUNCE_SAMPLES; i++) {
    if (k->key_debounce_samples[i] != k->key_debounce_samples[i - 1]) {
      debounced = false;
      break;
    }
  }

  if (debounced) {
    k->key = keyboard_scale_value(value);
    k->current_state = KEYBOARD_STABLE;
  } else {
    k->current_state = KEYBOARD_UNSTABLE;
  }
}

