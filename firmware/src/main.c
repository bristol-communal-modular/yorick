#define F_CPU 8000000

#include <stdlib.h>
#include <stdbool.h>

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include "yorick.h"

#include "oscillator.h"
#include "pitches.h"
#include "led_control.h"
#include "ticker.h"
#include "sequencer.h"
#include "button.h"
#include "control_pot.h"
#include "envelope.h"
#include "keyboard.h"
#include "wavetables.h"
#include "param_manager.h"


#define MAX_ADC_VALUE 1023

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

uint16_t osc1_tuning;


void startADCConversion(uint8_t adc_channel) {
  ADMUX  = adc_channel;
  ADCSRA |= _BV(ADSC);
}

volatile Oscillator osc1;
volatile Oscillator lfo;

Envelope volume_env;
Envelope filter_env;

ParamManager param_manager;

ControlPot pot1;
ControlPot pot2;

Button button1;
Button button2;

Keyboard keyboard;

LEDControl led1;
LEDControl led2;

Ticker clock;

Sequencer sequencer;

const uint8_t *osc_wavetable;
const uint8_t *lfo_wavetable;
bool env_out;

// takes the 0 to 1023 mod value and helps make a more logarithmic control
#define ctrl_log_curve(value, bp, mult) \
  (value < bp) ? value : (((value - bp) * mult) + bp)

void set_osc_wave(uint8_t value) {
  switch (value) {
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
}

void set_lfo_wave(uint8_t value) {
  env_out = false;
  switch (value) {
    case 0:
      lfo_wavetable = WT_SINE;
      break;
    case 1:
      lfo_wavetable = WT_SAW;
      break;
    case 2:
      env_out = true;
      break;
    case 3:
      lfo_wavetable = WT_RANDOM;
      break;
  }
}

void set_parameter(ParamType control, uint16_t value) {
  uint16_t tmp;
  switch(control) {
    case PARAM_TUNING:
      // constrain octave to between 0 and 127
      osc1_tuning = (value >> 3);
      break;
    case PARAM_OSC_WAVE:
      set_osc_wave(value >> 8);
      break;
    case PARAM_LFO_WAVE:
      set_lfo_wave(value >> 8);
      break;
    case PARAM_LFO_RATE:
      tmp = ctrl_log_curve(value, 400, 10);
      osc_set_pitch(lfo, (tmp >> 4) + 1);
      tmp = MAX_ADC_VALUE - value; // reverse pot
      tmp = ctrl_log_curve(tmp, 400, 5);
      envelope_set_decay(&filter_env, (tmp>>2) + 10);
      break;
    case PARAM_ENVELOPE_ATTACK:
      tmp = MAX_ADC_VALUE - value; // reverse pot
      tmp = ctrl_log_curve(tmp, 400, 5);
      envelope_set_attack(&volume_env, (tmp>>2) + 10);
      envelope_set_attack(&filter_env, (tmp>>2) + 10);
      break;
    case PARAM_ENVELOPE_DECAY:
      tmp = MAX_ADC_VALUE - value; // reverse pot
      tmp = ctrl_log_curve(tmp, 400, 5);
      envelope_set_decay(&volume_env, (tmp>>2) + 10);
      break;
    default:
      break;
  }
}

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

  envelope_init(&volume_env, 20);
  envelope_init(&filter_env, 20);

  control_pot_init(&pot1);
  control_pot_init(&pot2);

  button_init(&button1, &clock);
  button_init(&button2, &clock);

  keyboard_init(&keyboard);

  led_control_init(&led1, &clock);
  led_control_init(&led2, &clock);

  sequencer_init(&sequencer, &clock);
  sequencer_set_step_length(&sequencer, 300);
  sequencer_set_note_length(&sequencer, 300);

  // Setup param_manager and set initial control values
  // All need to be in the range 0 to 1023
  // The param_manager is responsible for scaling and shifting them
  param_manager_init(&param_manager);
  set_parameter(PARAM_TUNING, 800);
  set_parameter(PARAM_OSC_WAVE, 0);
  set_parameter(PARAM_LFO_RATE, 100);
  set_parameter(PARAM_LFO_WAVE, 0);
  set_parameter(PARAM_ENVELOPE_ATTACK, 100);
  set_parameter(PARAM_ENVELOPE_DECAY, 300);

  freq_adc_in = 0;
  mod1_adc_in = 0;
  mod2_adc_in = 0;

  uint8_t freq_lookup = 0;

  osc_set_pitch(osc1, 0);

  env_out = false;

  YorickMode mode = YORICK_PLAY_MODE;
  led1.led_on = true;

  while (1) {

    button_update(
      &button1,
      (PINA & _BV(BUTTON_1_IN_PIN)) ? BUTTON_UP : BUTTON_PRESSED
    );
    button_update(
      &button2,
      (PINA & _BV(BUTTON_2_IN_PIN)) ? BUTTON_UP : BUTTON_PRESSED
    );

    keyboard_update(&keyboard, freq_adc_in);

    control_pot_update(&pot1, (mod1_adc_in >> 1) << 1);
    control_pot_update(&pot2, (mod2_adc_in >> 1) << 1);

    led_control_update(&led1);
    led_control_update(&led2);

    if (button_just_released(&button1)) {
      control_pot_lock(&pot1);
      control_pot_lock(&pot2);
      if (mode == YORICK_PLAY_MODE) {
        mode = YORICK_SEQUENCER_MODE;
        led1.led_on = false;
      } else if (mode == YORICK_SEQUENCER_MODE) {
        mode = YORICK_PLAY_MODE;
        led1.led_on = true;
      }
    }

    if (sequencer.running) {
      freq_lookup = sequencer_current_step_value(&sequencer) + osc1_tuning + keyboard_get_key(&keyboard);
      osc_set_pitch(osc1, pgm_read_word(&MIDI_NOTE_PITCHES[freq_lookup]));

      if (sequencer_note_started(&sequencer)) {
        led_control_flash_start(&led1, 5, 1);
        envelope_start(&volume_env);
        envelope_start(&filter_env);
      }
      if (sequencer_note_finished(&sequencer)) {
        envelope_release(&volume_env);
        envelope_release(&filter_env);
      }
      // make sure to tick after checking for started notes
      // otherwise it's possible to miss the first note when
      // the sequencer is just started
      sequencer_tick(&sequencer);
    }

    if (mode == YORICK_PLAY_MODE) {

      if (!control_pot_is_locked(&pot1) && control_pot_changed(&pot1)) {
        set_parameter(param_manager_current(&param_manager, 0), control_pot_value(&pot1));
      }

      if (!control_pot_is_locked(&pot2) && control_pot_changed(&pot2)) {
        set_parameter(param_manager_current(&param_manager, 1), control_pot_value(&pot2));
      }

      if (button_just_released(&button2)) {
        param_manager_next_bank(&param_manager);
        control_pot_lock(&pot1);
        control_pot_lock(&pot2);
        led_control_flash_start(&led2, param_manager.bank + 1, 15);
      }

      if (!sequencer.running) {
        // Check if stable, not if_pressed here otherwise changes
        // to osc1_tuning won't be heard as the key is held
        if (keyboard_stable(&keyboard)) {
          freq_lookup = keyboard_get_key(&keyboard) + osc1_tuning;
          osc_set_pitch(osc1, pgm_read_word(&MIDI_NOTE_PITCHES[freq_lookup]));
        }

        if (keyboard_key_pressed(&keyboard)) {
          envelope_start(&volume_env);
          envelope_start(&filter_env);
        }

        if (keyboard_key_released(&keyboard)) {
          envelope_release(&volume_env);
          envelope_release(&filter_env);
        }
      }

    } else if (mode == YORICK_SEQUENCER_MODE) {

      if (!control_pot_is_locked(&pot1) && control_pot_changed(&pot1)) {
        sequencer_set_step_length(&sequencer, (1<<10) - control_pot_value(&pot1));
      }

      if (!control_pot_is_locked(&pot2) && control_pot_changed(&pot2)) {
        // change control pot value to be uint8
        sequencer_set_note_length(&sequencer, control_pot_value(&pot2));
      }


      if (button_is_held(&button2)) {
        sequencer_clear(&sequencer);
        led_control_flash_start(&led2, 10, 2);
        button_reset(&button2);
        envelope_release(&volume_env);
        envelope_release(&filter_env);
      }

      if (button_just_released(&button2)) {
        if (sequencer.running) {
          sequencer_stop(&sequencer);
          envelope_release(&volume_env);
          envelope_release(&filter_env);
        } else if (sequencer.step_count > 0) {
          led_control_flash_start(&led1, 1, 2);
          sequencer_start(&sequencer);
          keyboard_reset_key(&keyboard);
        }
      }

      if (sequencer.editable) {
        if (keyboard_key_pressed(&keyboard)) {
          led_control_flash_start(&led1, 1, 3);
          sequencer_add_step(&sequencer, keyboard_get_key(&keyboard));
          freq_lookup = keyboard_get_key(&keyboard) + osc1_tuning;
          osc_set_pitch(osc1, pgm_read_word(&MIDI_NOTE_PITCHES[freq_lookup]));
          envelope_start(&volume_env);
          envelope_start(&filter_env);
        }

        if (keyboard_key_released(&keyboard)) {
          envelope_release(&volume_env);
          envelope_release(&filter_env);
        }
      }

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

  ticker_tick(&clock);
  envelope_tick(&volume_env);
  osc_update(osc1);

  uint8_t osc_wave = pgm_read_byte(&osc_wavetable[osc_8bit_value(osc1)]);

  uint8_t ve = envelope_8bit_value(&volume_env);

  OCR1B = ((uint16_t)osc_wave * (uint16_t)ve) >> 8;

  if (env_out) {
    envelope_tick(&filter_env);
    OCR1A = envelope_8bit_value(&filter_env);
  } else {
    osc_update(lfo);
    OCR1A = pgm_read_byte(&lfo_wavetable[osc_8bit_value(lfo)]);
  }
}
