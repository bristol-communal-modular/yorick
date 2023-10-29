#pragma once

#include <stdint.h>
#include <stdbool.h>

#define KEYBOARD_DEBOUNCE_SAMPLES 10
#define KEYBOARD_MIN_VALUE 30

typedef enum {
  KEYBOARD_STABLE,
  KEYBOARD_UNSTABLE,
} KeyboardStableState;

typedef enum {
  KEYBOARD_PRESSED,
  KEYBOARD_NOT_PRESSED,
} KeyboardPressedState;

typedef struct Keyboard {
  KeyboardStableState current_stable_state;

  KeyboardPressedState current_pressed_state;
  KeyboardPressedState previous_pressed_state;

  uint16_t filtered_value;

  uint8_t key;
  uint8_t key_debounce_samples[KEYBOARD_DEBOUNCE_SAMPLES];
  uint8_t key_debounce_count;
} Keyboard;


void keyboard_init(Keyboard *k);

void keyboard_update(Keyboard *k, uint16_t value);

#define keyboard_get_key(k) (k)->key

#define keyboard_reset_key(k) (k)->key = 0;

#define keyboard_stable(k) ((k)->current_stable_state == KEYBOARD_STABLE)

#define keyboard_unstable(k) ((k)->current_stable_state == KEYBOARD_UNSTABLE)

#define keyboard_key_pressed(k) \
  ((k)->previous_pressed_state == KEYBOARD_NOT_PRESSED && (k)->current_pressed_state == KEYBOARD_PRESSED)

#define keyboard_key_released(k) \
  ((k)->previous_pressed_state == KEYBOARD_PRESSED && (k)->current_pressed_state == KEYBOARD_NOT_PRESSED)
