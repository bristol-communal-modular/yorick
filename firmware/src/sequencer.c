#include "sequencer.h"

void sequencer_init(Sequencer *s) {
  s->running = false;
  s->speed = 10;
  s->step_count = 0;
  s->current_step = 0;
  for (uint8_t i = 0; i < SEQUENCER_MAX_STEPS; i++) {
    s->steps[i] = 0;
  }
}

void sequencer_tick(Sequencer *s) {
}

void sequencer_add_step(Sequencer *s, uint8_t note) {
  if (s->step_count > (SEQUENCER_MAX_STEPS - 1)) {
    return;
  }
  s->step_count += 1;
  s->steps[s->step_count] = note;
}
