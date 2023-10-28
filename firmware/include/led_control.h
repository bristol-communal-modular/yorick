#include <stdbool.h>

#include "ticker.h"

typedef enum {
  LED_STATE_OFF,
  LED_STATE_ON
} LEDState;

typedef struct LEDControl {
  LEDState base_state;
  LEDState state;

  bool flashing;
  uint8_t changes_remaining;
  uint8_t interval;

  uint8_t delta;
  uint8_t last_tick;
  Ticker *ticker;

} LEDControl;

void led_control_init(LEDControl *led, Ticker *t);

#define led_control_reset(led) led_control_init(led, (led)->ticker)

void led_control_flash_start(LEDControl *led, uint8_t flashes, uint8_t interval);

void led_control_update(LEDControl *led);

#define led_control_set_base_off(led) (led)->base_state = LED_STATE_OFF

#define led_control_set_base_on(led) (led)->base_state = LED_STATE_ON

#define led_control_toggle_flash(led) (led)->state = ((led)->state == LED_STATE_ON) ? LED_STATE_OFF : LED_STATE_ON

#define led_control_flash_state(led) ((led)->base_state == LED_STATE_OFF) ? LED_STATE_ON : LED_STATE_OFF

#define led_control_turn_on(led) (led)->state = LED_STATE_ON

#define led_control_turn_off(led) (led)->state = LED_STATE_OFF

#define led_control_is_on(led) (led)->state == LED_STATE_ON

#define led_control_is_off(led) (led)->state == LED_STATE_OFF
