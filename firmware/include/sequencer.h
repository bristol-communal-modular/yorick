#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "ticker.h"

#define SEQUENCER_MAX_STEPS 16

typedef enum {
  SEQUENCER_NOTE_ON,
  SEQUENCER_NOTE_OFF,
} SequencerNoteState;

typedef struct Sequencer {
  bool running;
  uint16_t last_tick;
  uint16_t step_delta;
  uint16_t ticks_per_step;

  Ticker *ticker;

  uint16_t note_state;
  uint16_t prev_note_state;
  uint16_t note_length;

  bool editable;

  uint8_t step_count;
  uint8_t current_step;
  uint16_t steps[SEQUENCER_MAX_STEPS];

} Sequencer;

#define sequencer_is_running(s) (s)->running

#define sequencer_clear(s) \
  (s)->running = false; \
  (s)->editable = true; \
  (s)->step_count = 0;

#define sequencer_set_step_length(s, value) \
  (s)->ticks_per_step = value;

#define sequencer_set_note_length(s, value) \
  (s)->note_length = value;

void sequencer_init(Sequencer *s, Ticker *ticker);

void sequencer_start(Sequencer *s);

void sequencer_stop(Sequencer *s);

void sequencer_reset(Sequencer *s);

#define sequencer_reset(s) \
  sequencer_stop(s); \
  sequencer_start(s);

void sequencer_next_step(Sequencer *s);

void sequencer_start_note(Sequencer *s);

void sequencer_stop_note(Sequencer *s);

void sequencer_tick(Sequencer *s);

void sequencer_add_step(Sequencer *s, uint8_t note);

#define sequencer_note_started(s) \
  ((s)->note_state == SEQUENCER_NOTE_ON && \
   (s)->prev_note_state == SEQUENCER_NOTE_OFF)

#define sequencer_note_finished(s) \
  ((s)->note_state == SEQUENCER_NOTE_OFF && \
   (s)->prev_note_state == SEQUENCER_NOTE_ON)

#define sequencer_update_prev_note_state(s) \
  (s)->prev_note_state = (s)->note_state
