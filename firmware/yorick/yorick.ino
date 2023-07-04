#define F_CPU 8000000

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

#define FREQ_IN_PIN A1
#define MOD_IN_PIN A2

#define BUTTON_1_IN_PIN 0
#define BUTTON_2_IN_PIN 3

#define LED_1_OUT_PIN 6
#define LED_2_OUT_PIN 8

#define OSC_OUT_PIN 5
#define LFO_OUT_PIN 7

//the actual table that is read to generate the sound
unsigned char wavetable[256];

void sawtoothWave() {
  for (int i = 0; i < 256; ++i) {
    wavetable[i] = i; // sawtooth
  }
}

unsigned int frequency;

void setup () {
  pinMode(LED_1_OUT_PIN, OUTPUT);
  pinMode(LED_2_OUT_PIN, OUTPUT);

  pinMode(OSC_OUT_PIN, OUTPUT);
  pinMode(LFO_OUT_PIN, OUTPUT);

  pinMode(BUTTON_1_IN_PIN, INPUT_PULLUP);
  pinMode(BUTTON_2_IN_PIN, INPUT_PULLUP);

  sawtoothWave();
}
unsigned int note_phases[12] = {
  10, 20, 30, 40,
  50, 60, 70, 80,
  90, 100, 110, 120};

bool freq_in = false;
bool play_sound = false;

bool mod_in = false;
bool b1_in = false;
bool b2_in = false;

void checkButton1In() {
  int in = digitalRead(BUTTON_1_IN_PIN);
  if (in == HIGH) {
    b1_in = true;
  } else {
    b1_in = false;
  }
}

void checkButton2In() {
  int in = digitalRead(BUTTON_2_IN_PIN);
  if (in == HIGH) {
    b2_in = true;
  } else {
    b2_in = false;
  }
}

unsigned int checkFreqIn() {
  float in = float(analogRead(FREQ_IN_PIN)) / 1023.0;
  if (in > 0.01) {
    play_sound = true;
  } else {
    play_sound = false;
  }
  return note_phases[int(in * 12.0)];
}

void checkModIn() {
  float in = float(analogRead(MOD_IN_PIN)) / 1023.0;
  if (in > 0.5) {
    mod_in = true;
  } else {
    mod_in = false;
  }
}

unsigned int phase = 0;

float lfo_phase = 0;
float lfo_phase_delta = 0.01;

void loop() {

  unsigned int phase_delta = checkFreqIn();
  phase += phase_delta;
  phase = phase && ((1 << 8) - 1);

  if (true) {
    analogWrite(OSC_OUT_PIN, sawtoothWave[phase]);
  } else {
    analogWrite(OSC_OUT_PIN, 0);
  }

  /*lfo_phase += lfo_phase_delta;*/
  /*analogWrite(LFO_OUT_PIN, sawtoothWave[int(lfo_phase)]);*/
  /*if (lfo_phase > 255) {*/
    /*lfo_phase = 0;*/
  /*}*/

  //delay(1);

  /*checkModIn();*/

  checkButton1In();
  checkButton2In();

  if (b1_in) {
    digitalWrite(LED_1_OUT_PIN, HIGH);
  } else {
    digitalWrite(LED_1_OUT_PIN, LOW);
  }

  if (b2_in) {
    digitalWrite(LED_2_OUT_PIN, HIGH);
  } else {
    digitalWrite(LED_2_OUT_PIN, LOW);
  }

}
