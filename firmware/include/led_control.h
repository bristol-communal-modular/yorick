#include <stdbool.h>

#include "ticker.h"

typedef enum {
  LED_STATE_OFF,
  LED_STATE_ON
} LEDState;

typedef struct LEDControl {
  LEDState state;

  bool flashing;
  uint8_t changes_remaining;
  uint8_t interval;

  uint8_t delta;
  uint8_t last_tick;
  Ticker *ticker;

} LEDControl;

void led_control_init(LEDControl *led, Ticker *t);

void led_control_flash_start(LEDControl *led, uint8_t flashes, uint8_t interval);

#define led_control_flash_stop(led) (led)->flashing = false

void led_control_update(LEDControl *led);

#define led_control_set_state(led, new_state) (led)->state = new_state

#define led_control_toggle(led) (led)->state = ((led)->state == LED_STATE_ON) ? LED_STATE_OFF : LED_STATE_ON

#define led_control_turn_on(led) (led)->state = LED_STATE_ON

#define led_control_turn_off(led) (led)->state = LED_STATE_OFF

#define led_control_is_on(led) (led)->state == LED_STATE_ON

#define led_control_is_off(led) (led)->state == LED_STATE_OFF
