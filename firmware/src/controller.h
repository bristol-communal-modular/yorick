#pragma once

#include <stdint.h>
#include <stdbool.h>

#define CONTROL_COUNT 3
#define CONTROL_FM_FREQ 0
#define CONTROL_FM_DEPTH 1
#define CONTROL_OCTAVE 2

#define UNLOCK_THRESH 50

typedef struct Controller {
  bool locked;
  uint16_t previous;
  uint8_t current;
  uint16_t values[CONTROL_COUNT];
} Controller;

void controller_init(Controller *c);

void controller_lock(Controller *c, uint16_t previous);

void controller_next_control(Controller *c);

void controller_set_current(Controller *c, uint16_t value);

void controller_set_control(Controller *c, uint8_t control, uint16_t value);

uint16_t controller_get_control(Controller *c, uint8_t control);
