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

void keyboard_update(Keyboard *k, uint16_t value) {
  k->previous_state = k->current_state;

  // make sure input isn't zero
  if (value < KEYBOARD_MIN_VALUE) {
    k->current_state = KEYBOARD_UNSTABLE;
    return;
  }
  value = ((value + 3) >> 6) - 1;

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
    k->key = value;
    k->current_state = KEYBOARD_STABLE;
  } else {
    k->current_state = KEYBOARD_UNSTABLE;
  }
}

