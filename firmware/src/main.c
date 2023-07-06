#define F_CPU 8000000

#include <stdbool.h>

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

//#define FREQ_IN_PIN A1
//#define MOD_IN_PIN A2

#define BUTTON_1_IN_PIN PA0
#define BUTTON_2_IN_PIN PA3

#define LED_1_OUT_PIN PA6
#define LED_2_OUT_PIN PB2

#define OSC_OUT_PIN PA5
#define LFO_OUT_PIN PA7

#define OSC_OUT_PWM OCR1B
#define LFO_OUT_PWM OCR0B

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

  TCCR1A |= _BV(COM1B1);              // Enable OSC Out Pin PWM
  TCCR1B |= _BV(CS10);                // No PWM prescaler

  /* Setup Pin In/Outs */
  DDRA |= _BV(LED_1_OUT_PIN); // LED 1 pin output
  DDRB |= _BV(LED_2_OUT_PIN); // LED 2 pin output
  DDRA |= _BV(OSC_OUT_PIN); // LED 1 pin output
  DDRB |= _BV(LFO_OUT_PIN); // LED 2 pin output
                              //
  DDRA &= ~_BV(BUTTON_1_IN_PIN); // Butt
  DDRA &= ~_BV(BUTTON_2_IN_PIN); // Button 2 pin input

  PORTA |= _BV(BUTTON_1_IN_PIN); // Button 1 pin pull up resistor
  PORTA |= _BV(BUTTON_2_IN_PIN); // Button 2 pin pull up resistor

  sei();            

  while (1) {

    _delay_ms(1);

    if (bit_is_clear(PINA, BUTTON_1_IN_PIN)) {
      PORTA |= _BV(LED_1_OUT_PIN);
    } else {
      PORTA &= ~_BV(LED_1_OUT_PIN);
    }

    if (bit_is_clear(PINA, BUTTON_2_IN_PIN)) {
      PORTB |= _BV(LED_2_OUT_PIN);
    } else {
      PORTB &= ~_BV(LED_2_OUT_PIN);
    }

  }

}

volatile unsigned int accumulator;
unsigned int note = 857; // Middle C

ISR( TIM0_COMPA_vect ) {
  accumulator = accumulator + note;
  OCR1B = (accumulator >> 8) & 0x80;
}
