#include <stdbool.h>

#include "led_control.h"
#include "ticker.h"

void led_control_init(LEDControl *f, Ticker *t) {
  f->running = false;
  f->led_on = false;
  f->flashes_remaining = 0;
  f->interval = 0;
  f->delta = 0;
  f->last_tick = 0;

  f->ticker = t;
}

void led_control_flash_start(LEDControl *f, uint8_t flashes, uint8_t interval) {
  f->flashes_remaining = flashes;
  f->led_on = true;
  f->running = true;
  f->interval = interval;
  f->delta = interval;
  f->last_tick = ticker_8bit_count(f->ticker);
}

void led_control_update(LEDControl *f) {
  if (!f->running) { return; }

  uint8_t now = ticker_8bit_count(f->ticker);
  uint8_t passed;
  if (now < f->last_tick) {
    // ticker count has overflowed
    passed = (UINT8_MAX - f->last_tick) + now;
  } else {
    passed = (now - f->last_tick);
  }

  if (passed < f->delta) {
    // still counting down
    f->delta = f->delta - passed;
    f->last_tick = now;
    return;
  }
  uint8_t diff = passed - f->delta;
  f->delta = f->interval - diff;

  f->last_tick = now - diff;

  if (f->led_on) {
    f->flashes_remaining -= 1;
    if (f->flashes_remaining < 1) {
      f->running = false;
    }
  }

  f->led_on = !f->led_on;
}
