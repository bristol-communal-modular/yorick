#include <stdint.h>
#include <stdbool.h>

#include "button.h"
#include "ticker.h"

void button_update(Button *b, ButtonState new_state) {
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

  if (button_just_let_go(b)) {
    b->time_pressed = 0;
    return;
  }
}

