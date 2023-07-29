#include "envelope.h"

void envelope_init(Envelope *e) {
  e->state = ENVELOPE_STOPPED;
  e->value = 0;
  e->attack = 0;
  e->decay = 0;
}

void envelope_set_attack(Envelope *e, uint16_t attack) {
  e->attack = attack;
}

void envelope_set_decay(Envelope *e, uint16_t decay) {
  e->decay = decay;
}

void envelope_start(Envelope *e) {
  e->state = ENVELOPE_RISING;
}

void envelope_tick(Envelope *e) {
  switch(e->state) {
    case ENVELOPE_RISING:
      if ((e->value + e->attack) < e->value) {
        // overflow would occur
        e->value = UINT16_MAX;
        e->state = ENVELOPE_FALLING;
      } else {
        e->value = e->value + e->attack;
      }
      break;
    case ENVELOPE_FALLING:
      if ((e->value - e->decay) > e->value) {
        // overflow would occur
        e->value = 0;
        e->state = ENVELOPE_STOPPED;
      } else {
        e->value = e->value + e->attack;
      }
      break;
    default:
      break;
  }
}

uint8_t envelope_8bit_value(Envelope *e) {
  return e->value >> 8;
}
