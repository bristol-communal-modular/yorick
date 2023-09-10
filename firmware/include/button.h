#pragma once

#include <stdint.h>

#include "ticker.h"

#define BUTTON_DEBOUNCE_SAMPLES 3

typedef enum {
  BUTTON_PRESSED,
  BUTTON_UP,
} ButtonState;

typedef struct Button {
  ButtonState current_state;
  ButtonState previous_state;

  ButtonState debounce_samples[BUTTON_DEBOUNCE_SAMPLES];
  ButtonState debounce_count;

  uint8_t last_press;
  uint8_t long_press_time;
  uint8_t time_pressed;
  Ticker *ticker;

} Button;

void button_init(Button *b, Ticker *t);

#define button_set_long_press_time(b, time) \
  (b)->long_press_time = time;

void button_update(Button *b, ButtonState new_state);

#define button_just_pressed(b) \
  ((b)->previous_state == BUTTON_UP && (b)->current_state == BUTTON_PRESSED)

#define button_just_released(b) \
  ((b)->previous_state == BUTTON_PRESSED && (b)->current_state == BUTTON_UP)

#define button_is_held(b) ((b)->current_state == BUTTON_PRESSED && (b)->time_pressed > (b)->long_press_time)

#define button_reset(b) \
  (b)->current_state = BUTTON_UP; \
  (b)->previous_state = BUTTON_UP; \
  (b)->time_pressed = 0;
