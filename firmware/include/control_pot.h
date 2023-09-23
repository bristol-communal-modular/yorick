#pragma once

#include <stdint.h>
#include <stdbool.h>

#define CONTROL_POT_UNLOCK_THRESH 50
#define CONTROL_POT_FILTER_SAMPLES 4

typedef struct ControlPot {
  bool locked;
  bool changed;

  uint16_t lock_value;
  uint16_t value;
  uint16_t filter_samples[CONTROL_POT_FILTER_SAMPLES];
  uint8_t filter_count;
} ControlPot;

void control_pot_init(ControlPot *cp);

void control_pot_update(ControlPot *cp, uint16_t new_value);

void control_pot_lock(ControlPot *cp);

#define control_pot_is_locked(cp) (cp)->locked == true

#define control_pot_changed(cp) (cp)->changed

#define control_pot_value(cp) (cp)->value
