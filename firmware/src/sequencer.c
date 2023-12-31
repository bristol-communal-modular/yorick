#include "sequencer.h"
#include "ticker.h"

void sequencer_init(Sequencer *s, Ticker *t) {
  s->running = false;

  s->last_tick = 0;
  s->step_delta = 0;
  s->ticks_per_step = 0;
  s->ticker = t;

  s->note_state = SEQUENCER_NOTE_STATE_OFF;
  s->prev_note_state = SEQUENCER_NOTE_STATE_OFF;
  s->note_length = 0;
  s->note_ticks = 0;

  s->editable = true;
  s->step_count = 0;
  s->current_step = 0;
  for (uint8_t i = 0; i < SEQUENCER_MAX_STEPS; i++) {
    s->steps[i] = 0;
    s->step_types[i] = SEQUENCER_NOTE_TYPE_TRIG;
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
  if (s->note_state == SEQUENCER_NOTE_STATE_OFF) {
    s->note_state = SEQUENCER_NOTE_STATE_ON;
    s->prev_note_state = SEQUENCER_NOTE_STATE_OFF;
  }
}

void sequencer_stop_note(Sequencer *s) {
  if (s->note_state == SEQUENCER_NOTE_STATE_ON) {
    s->note_state = SEQUENCER_NOTE_STATE_OFF;
    s->prev_note_state = SEQUENCER_NOTE_STATE_ON;
  }
}

void sequencer_tick(Sequencer *s) {
  if (!s->running) return;

  uint16_t now = ticker_count(s->ticker);
  uint16_t delta = now - s->last_tick;
  s->step_delta += delta;
  s->last_tick = now;

  sequencer_update_prev_note_state(s);

  if (s->note_state == SEQUENCER_NOTE_STATE_OFF) {
    if (s->step_delta < s->ticks_per_step) return;
    s->step_delta -= s->ticks_per_step;
    sequencer_start_note(s);
    sequencer_next_step(s);
  } else if (s->note_state == SEQUENCER_NOTE_STATE_ON) {
    if (s->step_delta < s->note_ticks) return;
    sequencer_stop_note(s);
  }
}

void sequencer_set_step_length(Sequencer *s, uint16_t value) {
  uint16_t logish = (value < 800) ? value : (((value - 800) * 4) + 800);
  s->ticks_per_step = 1000 + (logish << 3);
  s->note_ticks = (s->ticks_per_step >> 8) * (s->note_length >> 2);
}

void sequencer_set_note_length(Sequencer *s, uint16_t value) {
  s->note_length = value;
  s->note_ticks = (s->ticks_per_step >> 8) * (s->note_length >> 2);
}

void sequencer_add_step(Sequencer *s, uint8_t note, SequencerNoteType note_type) {
  if (s->step_count >= SEQUENCER_MAX_STEPS) return;

  s->steps[s->step_count] = note;
  s->step_types[s->step_count] = note_type;
  s->step_count += 1;
}
