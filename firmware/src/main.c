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
#include "edgedetector.h"
#include "wavetables.h"

#define FREQ_IN_ADC 0
#define FREQ_IN_MUX MUX0
#define MOD_IN_ADC 1
#define MOD_IN_MUX MUX1

#define BUTTON_1_IN_PIN PA0
#define BUTTON_2_IN_PIN PA3

#define LED_1_OUT_PIN PA7
#define LED_2_OUT_PIN PB2

#define OSC_OUT_PIN PA5
#define LFO_OUT_PIN PA6

#define OSC_OUT_PWM OCR1B
#define LFO_OUT_PWM OCR0B

uint8_t adc_read_channel = FREQ_IN_MUX;
uint16_t analog_values[2];


void startADCConversion(uint8_t adc_channel) {
  ADMUX  = _BV(adc_channel);
  ADCSRA |= _BV(ADSC);
}

uint8_t octave = 48;

volatile Oscillator osc1;
volatile Oscillator modOsc;
volatile Oscillator lfo;

Controller controller;

EdgeDetector button1;
EdgeDetector button2;

LEDFlasher led1;
LEDFlasher led2;

volatile Ticker ticker1;
volatile Ticker ticker2;

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
  osc_init(modOsc);
  osc_init(lfo);

  edge_detector_init(button1, (PINA & _BV(BUTTON_1_IN_PIN)));
  edge_detector_init(button2, (PINA & _BV(BUTTON_1_IN_PIN)));

  flash_init(&led1, &ticker1);
  flash_init(&led2, &ticker2);

  controller_init(&controller);
  controller_set_control(&controller, CONTROL_OCTAVE, 800);

  osc_set_pitch(lfo, 10);

  while (1) {

    controller_set_current(&controller, analog_values[MOD_IN_ADC]);

    edge_detector_update(button1, (PINA & _BV(BUTTON_1_IN_PIN)));
    edge_detector_update(button2, (PINA & _BV(BUTTON_2_IN_PIN)));
    flash_update(&led1);
    flash_update(&led2);

    if (edge_detector_is_rising(button1)) {
      controller_next_control(&controller);
      flash_start(&led1, controller.current + 1, 2000);
    }

    if (edge_detector_is_rising(button2)) {
      flash_start(&led2, controller_get_control(&controller, CONTROL_OCTAVE), 500);
    }

    if (led1.led_on) {
      PORTA |= _BV(LED_1_OUT_PIN);
    } else {
      PORTA &= ~_BV(LED_1_OUT_PIN);
    }

    if (led2.led_on) {
      PORTB |= _BV(LED_2_OUT_PIN);
    } else {
      PORTB &= ~_BV(LED_2_OUT_PIN);
    }

    uint8_t lookup = ((analog_values[FREQ_IN_ADC]>> 6) + controller_get_control(&controller, CONTROL_OCTAVE));

    osc_set_pitch(osc1, pgm_read_word(&MIDI_NOTE_PITCHES[lookup]));
    osc_set_pitch(modOsc, pgm_read_word(&MIDI_NOTE_PITCHES[lookup]));
    osc_fm(modOsc, controller_get_control(&controller, CONTROL_FM_FREQ));

  }

}

ISR(ADC_vect) {
  uint16_t adc_value = ADCL;
  adc_value |= (ADCH<<8);

  switch(adc_read_channel) {
    case FREQ_IN_MUX:
      analog_values[FREQ_IN_ADC] = adc_value;
      adc_read_channel = MOD_IN_MUX;
      break;
    case MOD_IN_MUX:
      analog_values[MOD_IN_ADC] = adc_value;
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

  osc_update(osc1);
  osc_update(modOsc);

  uint16_t offset = (osc_8bit_value(modOsc) * controller_get_control(&controller, CONTROL_FM_DEPTH)) >> 3;
  osc_fm(osc1, offset);
  uint8_t out = pgm_read_byte(&WT_SINE[osc_8bit_value(osc1)]);

  if (analog_values[FREQ_IN_ADC] > 10) {
    OCR1B = out;
  } else {
    OCR1B = 128;
  }

  osc_update(lfo);
  uint8_t lfo_out = pgm_read_byte(&WT_SINE[osc_8bit_value(lfo)]);
  OCR1A = lfo_out;
}
