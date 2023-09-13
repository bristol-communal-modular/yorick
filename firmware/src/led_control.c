#include <stdbool.h>

#include "led_control.h"
#include "ticker.h"

void led_control_init(LEDControl *led, Ticker *t) {
  led->running = false;
  led->led_on = false;
  led->flashes_remaining = 0;
  led->interval = 0;
  led->delta = 0;
  led->last_tick = 0;

  led->ticker = t;
}

void led_control_flash_start(LEDControl *led, uint8_t flashes, uint8_t interval) {
  led->flashes_remaining = flashes;
  led->led_on = true;
  led->running = true;
  led->interval = interval;
  led->delta = interval;
  led->last_tick = ticker_8bit_count(led->ticker);
}

void led_control_update(LEDControl *led) {
  if (!led->running) { return; }

  uint8_t now = ticker_8bit_count(led->ticker);
  uint8_t passed;
  if (now < led->last_tick) {
    // ticker count has overflowed
    passed = (UINT8_MAX - led->last_tick) + now;
  } else {
    passed = (now - led->last_tick);
  }

  if (passed < led->delta) {
    // still counting down
    led->delta = led->delta - passed;
    led->last_tick = now;
    return;
  }
  uint8_t diff = passed - led->delta;
  led->delta = led->interval - diff;

  led->last_tick = now - diff;

  if (led->led_on) {
    led->flashes_remaining -= 1;
    if (led->flashes_remaining < 1) {
      led->running = false;
    }
  }

  led->led_on = !led->led_on;
}
