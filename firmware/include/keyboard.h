#pragma once

#include <stdint.h>
#include <stdbool.h>

#define KEYBOARD_DEBOUNCE_SAMPLES 10
#define KEYBOARD_MIN_VALUE 30

typedef enum {
  KEYBOARD_STABLE,
  KEYBOARD_UNSTABLE,
} KeyboardState;

typedef struct Keyboard {
  KeyboardState current_state;
  KeyboardState previous_state;

  uint8_t key;
  uint8_t key_debounce_samples[KEYBOARD_DEBOUNCE_SAMPLES];
  uint8_t key_debounce_count;
} Keyboard;


void keyboard_init(Keyboard *k);

void keyboard_update(Keyboard *k, uint16_t value);

#define keyboard_get_key(k) (k)->key

#define keyboard_reset_key(k) (k)->key = 0;

#define keyboard_stable(k) ((k)->current_state == KEYBOARD_STABLE)

#define keyboard_unstable(k) ((k)->current_state == KEYBOARD_UNSTABLE)

#define keyboard_key_pressed(k) \
  ((k)->previous_state == KEYBOARD_UNSTABLE && (k)->current_state == KEYBOARD_STABLE)

#define keyboard_key_released(k) \
  ((k)->previous_state == KEYBOARD_STABLE && (k)->current_state == KEYBOARD_UNSTABLE)
