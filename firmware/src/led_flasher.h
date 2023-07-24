#include <stdbool.h>

typedef struct LEDFlasher {
  bool running;
  bool led_on;
  int flashes_remaining;
  int count;
  int interval;
} LEDFlasher;

void flash_init(LEDFlasher *f);

void flash_start(LEDFlasher *f, int flashes, int interval);

void flash_update(LEDFlasher *f);
