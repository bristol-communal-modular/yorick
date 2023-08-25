#include "sequencer.h"

void sequencer_init(Sequencer *s) {
  s->running = false;

  s->last_tick = 0;
  s->tick_delta = 0;
  s->ticks_per_step = 0;

  s->note_state = SEQUENCER_NOTE_OFF;
  s->prev_note_state = SEQUENCER_NOTE_OFF;
  s->note_length = 0;

  s->step_count = 0;
  s->current_step = 0;
  for (uint8_t i = 0; i < SEQUENCER_MAX_STEPS; i++) {
    s->steps[i] = 0;
  }
}

void sequencer_tick(Sequencer *s, uint16_t tick) {
  if (!s->running) return;
  uint16_t delta = s->tick_delta + tick;
  if (delta >= s->ticks_per_step) {
    s->tick_delta = delta - s->ticks_per_step;
    s->note_state = SEQUENCER_NOTE_ON;
    s->current_step += 1;
    if (s->current_step >= s->step_count) {
      s->current_step = 0;
    }
  }

  if (
      s->note_state == SEQUENCER_NOTE_ON &&
      s->tick_delta >= s->note_length
     ) {
      s->note_state = SEQUENCER_NOTE_OFF;
  }
}

bool sequencer__note(Sequencer *s) {
  if (
      s->note_state == SEQUENCER_NOTE_ON &&
      s->prev_note_state == SEQUENCER_NOTE_OFF) {
    s->prev_note_state = SEQUENCER_NOTE_ON;
    return true;
  }
  return false;
}



void sequencer_add_step(Sequencer *s, uint8_t note) {
  if (s->step_count > (SEQUENCER_MAX_STEPS - 1)) {
    return;
  }
  s->step_count += 1;
  s->steps[s->step_count] = note;
}
