#pragma once

#include <stdint.h>
#include <stdbool.h>

#define CONTROL_COUNT 7

typedef enum {
  CONTROL_TUNING,
  CONTROL_FM_FREQ,
  CONTROL_FM_DEPTH,
  CONTROL_ATTACK,
  CONTROL_DECAY,
  CONTROL_MOD_OUTPUT,
  CONTROL_LFO_SPEED,
} ControlType;

#define UNLOCK_THRESH 50

#define DEBOUNCE_SAMPLES 6

typedef struct Controller {
  bool locked;
  uint16_t previous;
  ControlType current;
  uint16_t values[CONTROL_COUNT];
  uint8_t freq;
  uint8_t freq_debounce_samples[DEBOUNCE_SAMPLES];
  uint8_t freq_debounce_count;
} Controller;

void controller_init(Controller *c);

void controller_lock(Controller *c, uint16_t previous);

void controller_next_control(Controller *c);

void controller_set_current(Controller *c, uint16_t value);

void controller_set_control(Controller *c, ControlType control, uint16_t value);

uint16_t controller_get_control(Controller *c, ControlType control);

void controller_set_freq(Controller *c, uint16_t value);

uint16_t controller_get_freq(Controller *c);
