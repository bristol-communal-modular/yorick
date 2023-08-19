#include "envelope.h"

void envelope_init(Envelope *e, uint8_t update_divider) {
  e->state = ENVELOPE_STOPPED;
  e->value = 0;
  e->attack = 0;
  e->decay = 0;
  e->update_divider = update_divider;
  e->update_counter = 0;
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

void envelope_release(Envelope *e) {
  e->state = ENVELOPE_FALLING;
}

void envelope_tick(Envelope *e) {
  e->update_counter += 1;
  if (e->update_counter < e->update_divider) {
    return;
  }
  e->update_counter = 0;

  switch(e->state) {
    case ENVELOPE_RISING:
      if ((e->value + e->attack) < e->value) {
        // overflow would occur
        e->value = UINT16_MAX;
        e->state = ENVELOPE_HOLDING;
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
        e->value = e->value - e->decay;
      }
      break;
    default:
      break;
  }
}

uint8_t envelope_8bit_value(Envelope *e) {
  return e->value >> 8;
}
