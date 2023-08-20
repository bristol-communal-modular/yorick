#pragma once

#include <stdint.h>
#include <stdbool.h>

#define SEQUENCER_MAX_STEPS 16

typedef struct Sequencer {
  bool running;
  uint8_t step_count;
  uint16_t speed;
  uint8_t current_step;
  uint16_t steps[SEQUENCER_MAX_STEPS];
} Sequencer;

void sequencer_init(Sequencer *s);

void sequencer_tick(Sequencer *s);

#define sequencer_start(sequencer) sequencer.running = true;

#define sequencer_stop(sequencer) sequencer.running = false;

#define sequencer_clear(sequencer) \
  sequencer.running = false; \
  sequencer.step_count = 0;

void sequencer_add_step(Sequencer *s, uint8_t note);
