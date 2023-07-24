#pragma once

#include <stdint.h>

#define CONTROL_COUNT 3
#define CONTROL_FM_FREQ 0
#define CONTROL_FM_DEPTH 1
#define CONTROL_OCTAVE 2

typedef struct Controller {
  int current;
  int values[CONTROL_COUNT];
} Controller;

void controller_init(Controller *c);

void controller_next_control(Controller *c);

void controller_set_current(Controller *c, int value);

void controller_set_control(Controller *c, int control, uint16_t value);

int controller_get_control(Controller *c, int control);
