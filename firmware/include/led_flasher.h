#include <stdbool.h>

#include "ticker.h"

typedef struct LEDFlasher {
  bool running;
  bool led_on;
  uint8_t flashes_remaining;
  uint8_t interval;
  uint8_t delta;
  uint8_t last_tick;

  Ticker *ticker;
} LEDFlasher;

void flash_init(LEDFlasher *f, Ticker *t);

void flash_start(LEDFlasher *f, uint8_t flashes, uint8_t interval);

void flash_update(LEDFlasher *f);
