#include <stdbool.h>

#include "led_control.h"
#include "ticker.h"

void led_control_init(LEDControl *led, Ticker *t) {
  led->state = LED_STATE_OFF;

  led->flashing = false;
  led->changes_remaining = 0;
  led->interval = 0;

  led->delta = 0;
  led->last_tick = 0;
  led->ticker = t;
}

void led_control_flash_start(LEDControl *led, uint8_t flashes, uint8_t interval) {
  led->flashing = true;

  if (led->state == LED_STATE_ON) {
    led->state = LED_STATE_OFF;
  } else {
    led->state = LED_STATE_ON;
  }
  // twice as many changes as flashes
  led->changes_remaining = flashes << 1;
  led->interval = interval;
  led->delta = interval;
  led->last_tick = ticker_8bit_count(led->ticker);
}

void led_control_update(LEDControl *led) {
  if (!led->flashing) { return; }

  uint8_t now = ticker_8bit_count(led->ticker);
  uint8_t passed = now - led->last_tick;

  if (passed < led->delta) {
    // still counting down
    led->delta = led->delta - passed;
    led->last_tick = now;
    return;
  }
  uint8_t diff = passed - led->delta;
  led->delta = led->interval - diff;

  led->last_tick = now - diff;

  if (led->state == LED_STATE_ON) {
    led->state = LED_STATE_OFF;
  } else {
    led->state = LED_STATE_ON;
  }

  led->changes_remaining -= 1;
  if (led->changes_remaining < 1) {
    led->flashing = false;
  }
}
