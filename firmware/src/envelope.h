#pragma once

#include <stdint.h>

typedef enum {
  ENVELOPE_STOPPED,
  ENVELOPE_RISING,
  ENVELOPE_FALLING,
} EnvelopeState;

typedef struct Envelope {
  EnvelopeState state;
  uint16_t value;
  uint16_t attack;
  uint16_t decay;
} Envelope;

void envelope_init(Envelope *e);

void envelope_set_attack(Envelope *e, uint16_t attack);

void envelope_set_decay(Envelope *e, uint16_t decay);

void envelope_start(Envelope *e);

void envelope_tick(Envelope *e);

uint8_t envelope_8bit_value(Envelope *e);
