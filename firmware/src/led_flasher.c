#include <stdbool.h>

typedef struct LEDFlasher {
  bool running;
  bool led_on;
  int flashes_remaining;
  int count;
  int interval;
} LEDFlasher;

void flash_init(LEDFlasher *f) {
  f->running = false;
  f->led_on = false;
  f->flashes_remaining = 0;
  f->count = 0;
  f->interval = 0;
}

void flash_start(LEDFlasher *f, int flashes, int interval) {
  f->flashes_remaining = flashes;
  f->led_on = true;
  f->running = true;
  f->interval = interval;
  f->count = f->interval;
}

void flash_update(LEDFlasher *f) {
  if (!f->running) { return; }

  f->count -= 1;
  if (f->count > 0) { return; }

  if (f->led_on) {
    f->flashes_remaining -= 1;
    if (f->flashes_remaining < 1) {
      f->running = false;
    }
  }

  f->led_on = !f->led_on;
  f->count = f->interval;
}
