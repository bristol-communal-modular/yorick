#define F_CPU 8000000

#include <stdlib.h>
#include <stdbool.h>

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include "controller.h"
#include "oscillator.h"
#include "pitches.h"
#include "led_flasher.h"
#include "ticker.h"
#include "envelope.h"
#include "edgedetector.h"
#include "wavetables.h"



#define FREQ_IN_MUX  0b00010
#define MOD_1_IN_MUX 0b00001
#define MOD_2_IN_MUX 0b00000

#define BUTTON_1_IN_PIN PA4
#define BUTTON_2_IN_PIN PA3

#define LED_1_OUT_PIN PA7
#define LED_2_OUT_PIN PB2

#define OSC_OUT_PIN PA5
#define LFO_OUT_PIN PA6

#define OSC_OUT_PWM OCR1B
#define LFO_OUT_PWM OCR0B

uint8_t adc_read_channel = FREQ_IN_MUX;

uint16_t volatile freq_adc_in;
uint16_t volatile mod1_adc_in;
uint16_t volatile mod2_adc_in;


void startADCConversion(uint8_t adc_channel) {
  ADMUX  = adc_channel;
  ADCSRA |= _BV(ADSC);
}

volatile Oscillator osc1;
volatile Oscillator lfo;

Envelope env;

Controller controller;

EdgeDetector button1;
EdgeDetector button2;

EdgeDetector volatile keyboard;

LEDFlasher led1;
LEDFlasher led2;

Ticker ticker1;
Ticker ticker2;

Ticker clock;

const int8_t *osc_wavetable;
const int8_t *lfo_wavetable;
bool env_out;

int main () {

  cli();                              // Disable inerrupts during setup

  /* Timer 0 Setup */
  // Timer0 gets setup to call the TIM_OVF interrupt at a 20kHz frequency

  TCCR0A |= _BV(WGM01) | _BV(WGM00);  // Enable Fast PWM mode
  TCCR0B |= _BV(WGM02);               // Overflow when the counter reaches TOP
  TCCR0B |= _BV(CS01);                // Use a /8 prescaler
  OCR0A = 49;                         // Divide by 50 (1M / 50 = 20k)

  TIMSK0 |= _BV(OCIE0A);              // Enable the compare match interrupt
  TIMSK0 &= ~_BV(TOIE0);              // Disable the overflow interrupt

  /* Timer 1 Setup */
  // Timer 1 will handle all the PWM

  TCCR1B |= _BV(WGM12);               // Enable 8bit Fast PWM mode
  TCCR1A |= _BV(WGM10);

  TCCR1A |= _BV(COM1A1);              // Enable LFO Out Pin PWM
  TCCR1A |= _BV(COM1B1);              // Enable OSC Out Pin PWM
  TCCR1B |= _BV(CS10);                // No PWM prescaler

  /* Setup ADC */
  ADMUX = 0;

  ADCSRA |= _BV(ADEN);               // Enable the ADC
  // ADC clock runs at main clock / 128
  ADCSRA |= _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);
  ADCSRA |= _BV(ADIE);               // Enable the conversion finish interrupt
  ADCSRB |= _BV(ADEN);               // Enable the ADC

  /* Setup Pin In/Outs */
  DDRA |= _BV(LED_1_OUT_PIN);        // LED 1 pin output
  DDRB |= _BV(LED_2_OUT_PIN);        // LED 2 pin output
  DDRA |= _BV(OSC_OUT_PIN);          // OSC pin output
  DDRA |= _BV(LFO_OUT_PIN);          // LFO pin output

  DDRA &= ~_BV(BUTTON_1_IN_PIN);     // Button 1 pin input
  DDRA &= ~_BV(BUTTON_2_IN_PIN);     // Button 2 pin input

  PORTA |= _BV(BUTTON_1_IN_PIN);     // Button 1 pin pull up resistor
  PORTA |= _BV(BUTTON_2_IN_PIN);     // Button 2 pin pull up resistor

  sei();            

  startADCConversion(adc_read_channel);

  osc_init(osc1);
  osc_init(lfo);

  envelope_init(&env, 20);

  edge_detector_init(button1, (PINA & _BV(BUTTON_1_IN_PIN)));
  edge_detector_init(button2, (PINA & _BV(BUTTON_1_IN_PIN)));

  edge_detector_init(keyboard, 0);

  flash_init(&led1, &ticker1);
  flash_init(&led2, &ticker2);

  // Setup controller and set initial control values
  // All need to be in the range 0 to 1023
  // The controller is responsible for scaling and shifting them
  controller_init(&controller);
  controller_set_control(&controller, CONTROL_TUNING, 800);
  controller_set_control(&controller, CONTROL_OSC_WAVE, 0);
  controller_set_control(&controller, CONTROL_LFO_RATE, 100);
  controller_set_control(&controller, CONTROL_LFO_WAVE, 0);
  controller_set_control(&controller, CONTROL_ENVELOPE_ATTACK, 800);
  controller_set_control(&controller, CONTROL_ENVELOPE_DECAY, 100);

  freq_adc_in = 0;
  mod1_adc_in = 0;
  mod2_adc_in = 0;

  env_out = false;

  while (1) {

    controller_set_current(&controller, mod1_adc_in);
    controller_set_freq(&controller, freq_adc_in);


    uint8_t freq_lookup = controller_get_freq(&controller) + controller_get_control(&controller, CONTROL_TUNING);
    osc_set_pitch(osc1, pgm_read_word(&MIDI_NOTE_PITCHES[freq_lookup]));

    switch (controller_get_control(&controller, CONTROL_OSC_WAVE)) {
      case 0:
        osc_wavetable = WT_SINE;
        break;
      case 1:
        osc_wavetable = WT_SAW;
        break;
      case 2:
        osc_wavetable = WT_SQUARE;
        break;
      case 3:
        osc_wavetable = WT_RANDOM;
        break;
    }

    switch (controller_get_control(&controller, CONTROL_LFO_WAVE)) {
      case 0:
        env_out = false;
        lfo_wavetable = WT_SINE;
        break;
      case 1:
        env_out = false;
        lfo_wavetable = WT_SAW;
        break;
      case 2:
        env_out = true;
        break;
      case 3:
        env_out = false;
        lfo_wavetable = WT_RANDOM;
        break;
    }

    osc_set_pitch(lfo, controller_get_control(&controller, CONTROL_LFO_RATE));

    envelope_set_attack(&env, controller_get_control(&controller, CONTROL_ENVELOPE_ATTACK));
    envelope_set_decay(&env, controller_get_control(&controller, CONTROL_ENVELOPE_DECAY));
    edge_detector_update(keyboard, freq_adc_in > 10);

    if (edge_detector_is_rising(keyboard)) {
      envelope_start(&env);
    }
    if (edge_detector_is_falling(keyboard)) {
      envelope_release(&env);
    }

    edge_detector_update(button1, (PINA & _BV(BUTTON_1_IN_PIN)));
    edge_detector_update(button2, (PINA & _BV(BUTTON_2_IN_PIN)));
    flash_update(&led1);
    flash_update(&led2);

    if (edge_detector_is_rising(button1)) {
      controller_next_control(&controller);
      flash_start(&led1, controller.current + 1, 3000);
    }

    if (edge_detector_is_rising(button2)) {
      flash_start(&led1, controller.current + 1, 3000);
    }

    if (led1.led_on) {
      PORTA |= _BV(LED_1_OUT_PIN);
    } else {
      PORTA &= ~_BV(LED_1_OUT_PIN);
    }

    if (env.state == ENVELOPE_STOPPED) {
      PORTB |= _BV(LED_2_OUT_PIN);
    } else {
      PORTB &= ~_BV(LED_2_OUT_PIN);
    }

  }

}

ISR(ADC_vect) {
  uint16_t adc_value = ADCL;
  adc_value |= (ADCH<<8);

  switch(adc_read_channel) {
    case FREQ_IN_MUX:
      freq_adc_in = adc_value;
      adc_read_channel = MOD_1_IN_MUX;
      break;
    case MOD_1_IN_MUX:
      mod1_adc_in = adc_value;
      adc_read_channel = MOD_2_IN_MUX;
      break;
    case MOD_2_IN_MUX:
      mod2_adc_in = adc_value;
      adc_read_channel = FREQ_IN_MUX;
      break;
    default:
      adc_read_channel = FREQ_IN_MUX;
  }

  startADCConversion(adc_read_channel);
}

ISR( TIM0_COMPA_vect ) {

  ticker_tick(&ticker1);
  ticker_tick(&ticker2);

  ticker_tick(&clock);

  envelope_tick(&env);

  osc_update(lfo);
  osc_update(osc1);

  uint8_t osc_wave = (pgm_read_byte(&osc_wavetable[osc_8bit_value(osc1)]) + 128);

  uint8_t envelope = envelope_8bit_value(&env);

  uint8_t out = ((uint16_t)osc_wave * (uint16_t)envelope) >> 8;

  OCR1B = out;

  if (env_out) {
    OCR1A = envelope;
  } else {
    uint8_t lfo_out = pgm_read_byte(&lfo_wavetable[osc_8bit_value(lfo)]);
    OCR1A = lfo_out + 128;
  }
}
