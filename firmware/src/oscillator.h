#pragma once

#include <stdlib.h>
#include <stdbool.h>

typedef struct Oscillator {
  uint16_t acc;
  uint16_t delta;
} Oscillator;

#define osc_init(name) \
  name.acc = 0;  \
  name.delta = 0;

#define osc_update(name) (&name)->acc = name.acc + name.delta;

#define osc_set_pitch(name, change) name.delta = change;

#define osc_value(name) name.acc

#define osc_8bit_value(name) (name.acc >> 8)

