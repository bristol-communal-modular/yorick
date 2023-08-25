#pragma once

#include <stdint.h>
#include <stdbool.h>

#define SEQUENCER_MAX_STEPS 16

typedef enum {
  SEQUENCER_NOTE_ON,
  SEQUENCER_NOTE_OFF,
} SequencerNoteState;

typedef struct Sequencer {
  bool running;
  uint16_t last_tick;
  uint16_t tick_delta;
  uint16_t ticks_per_step;

  uint16_t note_state;
  uint16_t prev_note_state;
  uint16_t note_length;

  uint8_t step_count;
  uint8_t current_step;
  uint16_t steps[SEQUENCER_MAX_STEPS];
} Sequencer;

void sequencer_init(Sequencer *s);

void sequencer_tick(Sequencer *s, uint16_t tick);

bool sequencer_trigger_note(Sequencer *s);

#define sequencer_start(sequencer, tick) \
  sequencer.running = true; \
  sequencer.last_tick = tick;

#define sequencer_stop(sequencer) \
  sequencer.running = false; \
  sequencer.tick_delta = 0;


#define sequencer_clear(sequencer) \
  sequencer.running = false; \
  sequencer.step_count = 0;

void sequencer_add_step(Sequencer *s, uint8_t note);
