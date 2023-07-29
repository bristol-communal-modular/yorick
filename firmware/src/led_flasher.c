#include <stdbool.h>

#include "led_flasher.h"
#include "ticker.h"

void flash_init(LEDFlasher *f, Ticker *t) {
  f->running = false;
  f->led_on = false;
  f->flashes_remaining = 0;
  f->interval = 0;

  f->ticker = t;
}

void flash_start(LEDFlasher *f, int flashes, int interval) {
  f->flashes_remaining = flashes;
  f->led_on = true;
  f->running = true;
  f->interval = interval;
  ticker_reset(f->ticker);
}

void flash_update(LEDFlasher *f) {
  if (!f->running) { return; }

  if (ticker_count(f->ticker) < f->interval) { return; }

  if (f->led_on) {
    f->flashes_remaining -= 1;
    if (f->flashes_remaining < 1) {
      f->running = false;
    }
  }

  f->led_on = !f->led_on;
  ticker_reset(f->ticker);
}
