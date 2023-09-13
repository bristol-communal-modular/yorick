#include <stdbool.h>

#include "ticker.h"

typedef struct LEDControl {
  bool running;
  bool led_on;
  uint8_t flashes_remaining;
  uint8_t interval;
  uint8_t delta;
  uint8_t last_tick;

  Ticker *ticker;
} LEDControl;

void led_control_init(LEDControl *led, Ticker *t);

void led_control_flash_start(LEDControl *led, uint8_t flashes, uint8_t interval);

void led_control_update(LEDControl *led);
