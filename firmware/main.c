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

#define OSC_OUT_PIN OCR1B

//#define LFO_OUT_PIN OCR1A

int main () {

  DDRA |= _BV(LED_1_OUT_PIN); // LED 1 pin output
  DDRB |= _BV(LED_2_OUT_PIN); // LED 2 pin output
                              //
  DDRA &= ~_BV(BUTTON_1_IN_PIN); // Button 1 pin input
  DDRA &= ~_BV(BUTTON_2_IN_PIN); // Button 2 pin input

  PORTA |= _BV(BUTTON_1_IN_PIN); // Button 1 pin pull up resistor
  PORTA |= _BV(BUTTON_2_IN_PIN); // Button 2 pin pull up resistor

  // Setup Fast PWM on timer 1
  //TCCR1A = 3<<WGM00;  // Fast PWM

  /*TCCR0B |= 2<<CS00;*/
  /*OCR0A = 49;   // Divide by 50 (1M / 50 = 20k)*/


  while (1) {
    //checkFreqIn();

    _delay_ms(1);

    /*checkModIn();*/

    /*checkButton1In();*/
    /*checkButton2In();*/

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

/*ISR( TIM0_OVF_vect ) {*/
  /*Acc = Acc + Note;*/
  /*OCR1B = (Acc >> 8) & 0x80;*/
/*}*/
