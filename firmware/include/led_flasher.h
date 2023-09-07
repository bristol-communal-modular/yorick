#include <stdbool.h>

#include "ticker.h"

typedef struct LEDFlasher {
  bool running;
  bool led_on;
  int flashes_remaining;
  int interval;

  Ticker *ticker;
} LEDFlasher;

void flash_init(LEDFlasher *f, Ticker *t);

void flash_start(LEDFlasher *f, int flashes, int interval);

void flash_update(LEDFlasher *f);
