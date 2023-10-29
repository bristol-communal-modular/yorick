#include "keyboard.h"

void keyboard_init(Keyboard *k) {
  k->current_stable_state = KEYBOARD_UNSTABLE;

  k->current_pressed_state = KEYBOARD_NOT_PRESSED;
  k->previous_pressed_state = KEYBOARD_NOT_PRESSED;

  k->filtered_value = 0;
  k->key = 0;
  for (uint8_t i = 0; i < KEYBOARD_DEBOUNCE_SAMPLES; i++) {
    k->key_debounce_samples[i] = 0;
  }
  k->key_debounce_count = 0;
}

void keyboard_update(Keyboard *k, uint16_t value) {
  k->previous_pressed_state = k->current_pressed_state;

  k->filtered_value = (value + k->filtered_value) >> 1;
  uint8_t scaled_value = (k->filtered_value) >> 6;

  k->key_debounce_count += 1;
  if (k->key_debounce_count >= KEYBOARD_DEBOUNCE_SAMPLES) {
    k->key_debounce_count = 0;
  }
  k->key_debounce_samples[k->key_debounce_count] = scaled_value;

  bool debounced = true;
  for (uint8_t i = 1; i < KEYBOARD_DEBOUNCE_SAMPLES; i++) {
    if (k->key_debounce_samples[i] != k->key_debounce_samples[i - 1]) {
      debounced = false;
      break;
    }
  }

  if (!debounced) {
    k->current_stable_state = KEYBOARD_UNSTABLE;
    return;
  }

  k->current_stable_state = KEYBOARD_STABLE;

  if (scaled_value > 0) {
    k->key = scaled_value;
    k->current_pressed_state = KEYBOARD_PRESSED;
  } else {
    k->current_pressed_state = KEYBOARD_NOT_PRESSED;
  }
}

