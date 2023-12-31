#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "ticker.h"

#define SEQUENCER_MAX_STEPS 16

typedef enum {
  SEQUENCER_NOTE_STATE_ON,
  SEQUENCER_NOTE_STATE_OFF,
} SequencerNoteState;

typedef enum {
  SEQUENCER_NOTE_TYPE_TRIG,
  SEQUENCER_NOTE_TYPE_HOLD,
  SEQUENCER_NOTE_TYPE_REST,
} SequencerNoteType;

typedef struct Sequencer {
  bool running;
  uint16_t last_tick;
  uint16_t step_delta;
  uint16_t ticks_per_step;

  Ticker *ticker;

  SequencerNoteState note_state;
  SequencerNoteState prev_note_state;
  uint16_t note_length;
  uint16_t note_ticks;

  bool editable;

  uint8_t step_count;
  uint8_t current_step;
  uint8_t steps[SEQUENCER_MAX_STEPS];
  SequencerNoteType step_types[SEQUENCER_MAX_STEPS];

} Sequencer;

#define sequencer_is_running(s) (s)->running

#define sequencer_clear(s) \
  (s)->running = false; \
  (s)->editable = true; \
  (s)->step_count = 0;

void sequencer_set_step_length(Sequencer *s, uint16_t value);

void sequencer_set_note_length(Sequencer *s, uint16_t value);

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

void sequencer_add_step(Sequencer *s, uint8_t note, SequencerNoteType note_type);

#define sequencer_add_trig_step(s, n) sequencer_add_step(s, n, SEQUENCER_NOTE_TYPE_TRIG)
#define sequencer_add_hold_step(s, n) sequencer_add_step(s, n, SEQUENCER_NOTE_TYPE_HOLD)
#define sequencer_add_rest_step(s, n) sequencer_add_step(s, n, SEQUENCER_NOTE_TYPE_REST)

#define sequencer_current_step_value(s) \
  (s)->steps[(s)->current_step]

#define sequencer_current_step_type(s) \
  (s)->step_types[(s)->current_step]

#define sequencer_get_step_value(s, value) \
  (s)->steps[value]

#define sequencer_note_started(s) \
  ((s)->note_state == SEQUENCER_NOTE_STATE_ON && \
   (s)->prev_note_state == SEQUENCER_NOTE_STATE_OFF && \
   ((s)->step_types[(s)->current_step] != SEQUENCER_NOTE_TYPE_REST))

#define sequencer_note_finished(s) \
  ((s)->note_state == SEQUENCER_NOTE_STATE_OFF && \
   (s)->prev_note_state == SEQUENCER_NOTE_STATE_ON && \
   ((s)->step_types[(s)->current_step] != SEQUENCER_NOTE_TYPE_HOLD))

#define sequencer_update_prev_note_state(s) \
  (s)->prev_note_state = (s)->note_state
