#pragma once

#include <stdint.h>

#include "ticker.h"

typedef enum {
  BUTTON_PRESSED,
  BUTTON_UP,
} ButtonState;

typedef struct Button {
  ButtonState current_state;
  ButtonState previous_state;

  uint8_t last_press;
  uint8_t long_press_time;
  uint8_t time_pressed;
  Ticker *ticker;

} Button;

#define button_init(b, t) \
  (b)->current_state = BUTTON_UP; \
  (b)->previous_state = BUTTON_UP; \
  (b)->last_press = 0; \
  (b)->time_pressed = 0; \
  (b)->long_press_time = 30; \
  (b)->ticker = t;

#define button_set_long_press_time(b, time) \
  (b)->long_press_time = time;

void button_update(Button *b, ButtonState new_state);

#define button_just_pressed(b) \
  ((b)->previous_state == BUTTON_UP && (b)->current_state == BUTTON_PRESSED)

#define button_just_let_go(b) \
  ((b)->previous_state == BUTTON_PRESSED && (b)->current_state == BUTTON_UP)

#define button_is_held(b) ((b)->current_state == BUTTON_PRESSED && (b)->time_pressed > (b)->long_press_time)

