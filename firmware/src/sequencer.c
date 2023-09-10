#include "sequencer.h"
#include "ticker.h"

void sequencer_init(Sequencer *s, Ticker *t) {
  s->running = false;

  s->last_tick = 0;
  s->step_delta = 0;
  s->ticks_per_step = 100;
  s->ticker = t;

  s->note_state = SEQUENCER_NOTE_OFF;
  s->prev_note_state = SEQUENCER_NOTE_OFF;
  s->note_length = 80;

  s->editable = true;
  s->step_count = 0;
  s->current_step = 0;
  for (uint8_t i = 0; i < SEQUENCER_MAX_STEPS; i++) {
    s->steps[i] = 0;
  }
}

void sequencer_start(Sequencer *s) {
  s->editable = false;
  s->running = true;
  s->step_delta = 0;
  s->current_step = 0;
  s->last_tick = ticker_count(s->ticker);
  sequencer_start_note(s);
}

void sequencer_stop(Sequencer *s) {
  s->running = false;
  s->step_delta = 0;
  s->current_step = 0;
  sequencer_stop_note(s);
}

void sequencer_next_step(Sequencer *s) {
  s->current_step += 1;
  if (s->current_step >= s->step_count) {
    s->current_step = 0;
  }
}

void sequencer_start_note(Sequencer *s) {
  if (s->note_state == SEQUENCER_NOTE_OFF) {
    s->note_state = SEQUENCER_NOTE_ON;
    s->prev_note_state = SEQUENCER_NOTE_OFF;
  }
}

void sequencer_stop_note(Sequencer *s) {
  if (s->note_state == SEQUENCER_NOTE_ON) {
    s->note_state = SEQUENCER_NOTE_OFF;
    s->prev_note_state = SEQUENCER_NOTE_ON;
  }
}

void sequencer_tick(Sequencer *s) {
  if (!s->running) return;

  uint16_t now = ticker_count(s->ticker);
  uint16_t delta = now - s->last_tick;
  s->step_delta += delta;
  s->last_tick = now;

  sequencer_update_prev_note_state(s);

  if (s->note_state == SEQUENCER_NOTE_OFF) {
    if (s->step_delta < s->ticks_per_step) return;

    s->step_delta -= s->ticks_per_step;
    sequencer_start_note(s);
    sequencer_next_step(s);
  } else if (s->note_state == SEQUENCER_NOTE_ON) {
    if (s->step_delta < s->note_length) return;

    sequencer_stop_note(s);
  }
}

void sequencer_add_step(Sequencer *s, uint8_t note) {
  if (s->step_count >= SEQUENCER_MAX_STEPS) return;

  s->steps[s->step_count] = note;
  s->step_count += 1;
}
