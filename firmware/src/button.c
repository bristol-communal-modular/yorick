#include <stdint.h>
#include <stdbool.h>

#include "button.h"
#include "ticker.h"

void button_init(Button *b, Ticker *t) {
  b->current_state = BUTTON_UP;
  b->previous_state = BUTTON_UP;
  b->last_press = 0;
  b->time_pressed = 0;
  b->long_press_time = 30;
  b->ticker = t;

  for (uint8_t i = 0; i < BUTTON_DEBOUNCE_SAMPLES; i++) {
    b->debounce_samples[i] = BUTTON_UP;
  }
  b->debounce_count = 0;
}

void button_update(Button *b, ButtonState new_state) {

  b->debounce_samples[b->debounce_count] = new_state;
  b->debounce_count += 1;
  if (b->debounce_count >= BUTTON_DEBOUNCE_SAMPLES) {
    b->debounce_count = 0;
  }

  for (uint8_t i = 1; i < BUTTON_DEBOUNCE_SAMPLES; i++) {
    if (b->debounce_samples[i] != b->debounce_samples[i - 1]) {
      return;
    }
  }

  b->previous_state = b->current_state;
  b->current_state = new_state;

  if (button_just_pressed(b)) {
    b->last_press = ticker_8bit_count(b->ticker);
    return;
  }

  if (b->current_state == BUTTON_PRESSED) {
    b->time_pressed = ticker_8bit_count(b->ticker) - b->last_press;
    return;
  }

  if (button_just_released(b)) {
    b->time_pressed = 0;
    return;
  }
}

