#include <avr/pgmspace.h>
#include <stdint.h>

/**
 * Sample rate is 20 kHz, and phase increments from 0 to 65535 in increments
 * of Pitch units every sample. This gives us the relation:
 *
 *   Frequency = (20,000 * Pitch) / 65535
 *                   or
 *       Pitch = Frequency * 65535 / 20,000
 *
 * This table defines this function for MIDI notes 0 to 127.
 */
const uint16_t MIDI_NOTE_PITCHES[128] PROGMEM = {
  // MIDI 0 - (ideal: 8.18 Hz) (actual: 7.93) (error: 52 cents)
  26,
  // MIDI 1 - (ideal: 8.66 Hz) (actual: 8.54) (error: 23 cents)
  28,
  // MIDI 2 - (ideal: 9.18 Hz) (actual: 9.16) (error: 4 cents)
  30,
  // MIDI 3 - (ideal: 9.72 Hz) (actual: 9.46) (error: 46 cents)
  31,
  // MIDI 4 - (ideal: 10.30 Hz) (actual: 10.07) (error: 38 cents)
  33,
  // MIDI 5 - (ideal: 10.91 Hz) (actual: 10.68) (error: 36 cents)
  35,
  // MIDI 6 - (ideal: 11.56 Hz) (actual: 11.29) (error: 40 cents)
  37,
  // MIDI 7 - (ideal: 12.25 Hz) (actual: 12.21) (error: 6 cents)
  40,
  // MIDI 8 - (ideal: 12.98 Hz) (actual: 12.82) (error: 21 cents)
  42,
  // MIDI 9 - (ideal: 13.75 Hz) (actual: 13.73) (error: 2 cents)
  45,
  // MIDI 10 - (ideal: 14.57 Hz) (actual: 14.34) (error: 27 cents)
  47,
  // MIDI 11 - (ideal: 15.43 Hz) (actual: 15.26) (error: 19 cents)
  50,
  // MIDI 12 - (ideal: 16.35 Hz) (actual: 16.17) (error: 18 cents)
  53,
  // MIDI 13 - (ideal: 17.32 Hz) (actual: 17.09) (error: 23 cents)
  56,
  // MIDI 14 - (ideal: 18.35 Hz) (actual: 18.31) (error: 3 cents)
  60,
  // MIDI 15 - (ideal: 19.45 Hz) (actual: 19.23) (error: 20 cents)
  63,
  // MIDI 16 - (ideal: 20.60 Hz) (actual: 20.45) (error: 12 cents)
  67,
  // MIDI 17 - (ideal: 21.83 Hz) (actual: 21.67) (error: 12 cents)
  71,
  // MIDI 18 - (ideal: 23.12 Hz) (actual: 22.89) (error: 17 cents)
  75,
  // MIDI 19 - (ideal: 24.50 Hz) (actual: 24.41) (error: 6 cents)
  80,
  // MIDI 20 - (ideal: 25.96 Hz) (actual: 25.94) (error: 1 cents)
  85,
  // MIDI 21 - A0  (ideal: 27.50 Hz) (actual: 27.47) (error: 2 cents)
  90,
  // MIDI 22 - A#0 (ideal: 29.14 Hz) (actual: 28.99) (error: 8 cents)
  95,
  // MIDI 23 - B0  (ideal: 30.87 Hz) (actual: 30.82) (error: 2 cents)
  101,
  // MIDI 24 - C1  (ideal: 32.70 Hz) (actual: 32.65) (error: 2 cents)
  107,
  // MIDI 25 - C#1 (ideal: 34.65 Hz) (actual: 34.48) (error: 8 cents)
  113,
  // MIDI 26 - D1  (ideal: 36.71 Hz) (actual: 36.62) (error: 4 cents)
  120,
  // MIDI 27 - D#1 (ideal: 38.89 Hz) (actual: 38.76) (error: 5 cents)
  127,
  // MIDI 28 - E1  (ideal: 41.20 Hz) (actual: 41.20) (error: 0 cents)
  135,
  // MIDI 29 - F1  (ideal: 43.65 Hz) (actual: 43.64) (error: 0 cents)
  143,
  // MIDI 30 - F#1 (ideal: 46.25 Hz) (actual: 46.08) (error: 6 cents)
  151,
  // MIDI 31 - G1  (ideal: 49.00 Hz) (actual: 48.83) (error: 6 cents)
  160,
  // MIDI 32 - G#1 (ideal: 51.91 Hz) (actual: 51.88) (error: 1 cents)
  170,
  // MIDI 33 - A1  (ideal: 55.00 Hz) (actual: 54.93) (error: 2 cents)
  180,
  // MIDI 34 - A#1 (ideal: 58.27 Hz) (actual: 57.98) (error: 8 cents)
  190,
  // MIDI 35 - B1  (ideal: 61.74 Hz) (actual: 61.65) (error: 2 cents)
  202,
  // MIDI 36 - C2  (ideal: 65.41 Hz) (actual: 65.31) (error: 2 cents)
  214,
  // MIDI 37 - C#2 (ideal: 69.30 Hz) (actual: 69.27) (error: 0 cents)
  227,
  // MIDI 38 - D2  (ideal: 73.42 Hz) (actual: 73.24) (error: 4 cents)
  240,
  // MIDI 39 - D#2 (ideal: 77.78 Hz) (actual: 77.51) (error: 5 cents)
  254,
  // MIDI 40 - E2  (ideal: 82.41 Hz) (actual: 82.40) (error: 0 cents)
  270,
  // MIDI 41 - F2  (ideal: 87.31 Hz) (actual: 87.28) (error: 0 cents)
  286,
  // MIDI 42 - F#2 (ideal: 92.50 Hz) (actual: 92.47) (error: 0 cents)
  303,
  // MIDI 43 - G2  (ideal: 98.00 Hz) (actual: 97.96) (error: 0 cents)
  321,
  // MIDI 44 - G#2 (ideal: 103.83 Hz) (actual: 103.76) (error: 1 cents)
  340,
  // MIDI 45 - A2  (ideal: 110.00 Hz) (actual: 109.86) (error: 2 cents)
  360,
  // MIDI 46 - A#2 (ideal: 116.54 Hz) (actual: 116.27) (error: 3 cents)
  381,
  // MIDI 47 - B2  (ideal: 123.47 Hz) (actual: 123.29) (error: 2 cents)
  404,
  // MIDI 48 - C3  (ideal: 130.81 Hz) (actual: 130.62) (error: 2 cents)
  428,
  // MIDI 49 - C#3 (ideal: 138.59 Hz) (actual: 138.55) (error: 0 cents)
  454,
  // MIDI 50 - D3  (ideal: 146.83 Hz) (actual: 146.79) (error: 0 cents)
  481,
  // MIDI 51 - D#3 (ideal: 155.56 Hz) (actual: 155.33) (error: 2 cents)
  509,
  // MIDI 52 - E3  (ideal: 164.81 Hz) (actual: 164.79) (error: 0 cents)
  540,
  // MIDI 53 - F3  (ideal: 174.61 Hz) (actual: 174.56) (error: 0 cents)
  572,
  // MIDI 54 - F#3 (ideal: 185.00 Hz) (actual: 184.94) (error: 0 cents)
  606,
  // MIDI 55 - G3  (ideal: 196.00 Hz) (actual: 195.92) (error: 0 cents)
  642,
  // MIDI 56 - G#3 (ideal: 207.65 Hz) (actual: 207.52) (error: 1 cents)
  680,
  // MIDI 57 - A3  (ideal: 220.00 Hz) (actual: 219.73) (error: 2 cents)
  720,
  // MIDI 58 - A#3 (ideal: 233.08 Hz) (actual: 232.85) (error: 1 cents)
  763,
  // MIDI 59 - B3  (ideal: 246.94 Hz) (actual: 246.89) (error: 0 cents)
  809,
  // MIDI 60 - C4 (middle C) (ideal: 261.63 Hz) (actual: 261.54) (error: 0 cents)
  857,
  // MIDI 61 - C#4 (ideal: 277.18 Hz) (actual: 277.10) (error: 0 cents)
  908,
  // MIDI 62 - D4  (ideal: 293.66 Hz) (actual: 293.58) (error: 0 cents)
  962,
  // MIDI 63 - D#4 (ideal: 311.13 Hz) (actual: 310.97) (error: 0 cents)
  1019,
  // MIDI 64 - E4  (ideal: 329.63 Hz) (actual: 329.59) (error: 0 cents)
  1080,
  // MIDI 65 - F4  (ideal: 349.23 Hz) (actual: 349.12) (error: 0 cents)
  1144,
  // MIDI 66 - F#4 (ideal: 369.99 Hz) (actual: 369.87) (error: 0 cents)
  1212,
  // MIDI 67 - G4  (ideal: 392.00 Hz) (actual: 391.85) (error: 0 cents)
  1284,
  // MIDI 68 - G#4 (ideal: 415.30 Hz) (actual: 415.04) (error: 1 cents)
  1360,
  // MIDI 69 - A4 (concert pitch) (ideal: 440.00 Hz) (actual: 439.76) (error: 0 cents)
  1441,
  // MIDI 70 - A#4 (ideal: 466.16 Hz) (actual: 466.00) (error: 0 cents)
  1527,
  // MIDI 71 - B4  (ideal: 493.88 Hz) (actual: 493.77) (error: 0 cents)
  1618,
  // MIDI 72 - C5  (ideal: 523.25 Hz) (actual: 523.07) (error: 0 cents)
  1714,
  // MIDI 73 - C#5 (ideal: 554.37 Hz) (actual: 554.20) (error: 0 cents)
  1816,
  // MIDI 74 - D5  (ideal: 587.33 Hz) (actual: 587.16) (error: 0 cents)
  1924,
  // MIDI 75 - D#5 (ideal: 622.25 Hz) (actual: 621.95) (error: 0 cents)
  2038,
  // MIDI 76 - E5  (ideal: 659.26 Hz) (actual: 659.18) (error: 0 cents)
  2160,
  // MIDI 77 - F5  (ideal: 698.46 Hz) (actual: 698.24) (error: 0 cents)
  2288,
  // MIDI 78 - F#5 (ideal: 739.99 Hz) (actual: 739.75) (error: 0 cents)
  2424,
  // MIDI 79 - G5  (ideal: 783.99 Hz) (actual: 783.69) (error: 0 cents)
  2568,
  // MIDI 80 - G#5 (ideal: 830.61 Hz) (actual: 830.38) (error: 0 cents)
  2721,
  // MIDI 81 - A5  (ideal: 880.00 Hz) (actual: 879.82) (error: 0 cents)
  2883,
  // MIDI 82 - A#5 (ideal: 932.33 Hz) (actual: 932.31) (error: 0 cents)
  3055,
  // MIDI 83 - B5  (ideal: 987.77 Hz) (actual: 987.55) (error: 0 cents)
  3236,
  // MIDI 84 - C6  (ideal: 1046.50 Hz) (actual: 1046.45) (error: 0 cents)
  3429,
  // MIDI 85 - C#6 (ideal: 1108.73 Hz) (actual: 1108.70) (error: 0 cents)
  3633,
  // MIDI 86 - D6  (ideal: 1174.66 Hz) (actual: 1174.62) (error: 0 cents)
  3849,
  // MIDI 87 - D#6 (ideal: 1244.51 Hz) (actual: 1244.51) (error: 0 cents)
  4078,
  // MIDI 88 - E6  (ideal: 1318.51 Hz) (actual: 1318.36) (error: 0 cents)
  4320,
  // MIDI 89 - F6  (ideal: 1396.91 Hz) (actual: 1396.79) (error: 0 cents)
  4577,
  // MIDI 90 - F#6 (ideal: 1479.98 Hz) (actual: 1479.80) (error: 0 cents)
  4849,
  // MIDI 91 - G6  (ideal: 1567.98 Hz) (actual: 1567.69) (error: 0 cents)
  5137,
  // MIDI 92 - G#6 (ideal: 1661.22 Hz) (actual: 1661.07) (error: 0 cents)
  5443,
  // MIDI 93 - A6  (ideal: 1760.00 Hz) (actual: 1759.95) (error: 0 cents)
  5767,
  // MIDI 94 - A#6 (ideal: 1864.66 Hz) (actual: 1864.62) (error: 0 cents)
  6110,
  // MIDI 95 - B6  (ideal: 1975.53 Hz) (actual: 1975.40) (error: 0 cents)
  6473,
  // MIDI 96 - C7  (ideal: 2093.00 Hz) (actual: 2092.90) (error: 0 cents)
  6858,
  // MIDI 97 - C#7 (ideal: 2217.46 Hz) (actual: 2217.41) (error: 0 cents)
  7266,
  // MIDI 98 - D7  (ideal: 2349.32 Hz) (actual: 2349.24) (error: 0 cents)
  7698,
  // MIDI 99 - D#7 (ideal: 2489.02 Hz) (actual: 2489.01) (error: 0 cents)
  8156,
  // MIDI 100 - E7  (ideal: 2637.02 Hz) (actual: 2636.72) (error: 0 cents)
  8640,
  // MIDI 101 - F7  (ideal: 2793.83 Hz) (actual: 2793.58) (error: 0 cents)
  9154,
  // MIDI 102 - F#7 (ideal: 2959.96 Hz) (actual: 2959.90) (error: 0 cents)
  9699,
  // MIDI 103 - G7  (ideal: 3135.96 Hz) (actual: 3135.68) (error: 0 cents)
  10275,
  // MIDI 104 - G#7 (ideal: 3322.44 Hz) (actual: 3322.14) (error: 0 cents)
  10886,
  // MIDI 105 - A7  (ideal: 3520.00 Hz) (actual: 3519.90) (error: 0 cents)
  11534,
  // MIDI 106 - A#7 (ideal: 3729.31 Hz) (actual: 3729.25) (error: 0 cents)
  12220,
  // MIDI 107 - B7  (ideal: 3951.07 Hz) (actual: 3950.81) (error: 0 cents)
  12946,
  // MIDI 108 - C8  (ideal: 4186.01 Hz) (actual: 4185.79) (error: 0 cents)
  13716,
  // MIDI 109 - C#8 (ideal: 4434.92 Hz) (actual: 4434.81) (error: 0 cents)
  14532,
  // MIDI 110 - D8  (ideal: 4698.64 Hz) (actual: 4698.49) (error: 0 cents)
  15396,
  // MIDI 111 - D#8 (ideal: 4978.03 Hz) (actual: 4978.03) (error: 0 cents)
  16312,
  // MIDI 112 - E8  (ideal: 5274.04 Hz) (actual: 5273.74) (error: 0 cents)
  17281,
  // MIDI 113 - F8  (ideal: 5587.65 Hz) (actual: 5587.46) (error: 0 cents)
  18309,
  // MIDI 114 - F#8 (ideal: 5919.91 Hz) (actual: 5919.80) (error: 0 cents)
  19398,
  // MIDI 115 - G8  (ideal: 6271.93 Hz) (actual: 6271.67) (error: 0 cents)
  20551,
  // MIDI 116 - G#8 (ideal: 6644.88 Hz) (actual: 6644.59) (error: 0 cents)
  21773,
  // MIDI 117 - A8  (ideal: 7040.00 Hz) (actual: 7039.79) (error: 0 cents)
  23068,
  // MIDI 118 - A#8 (ideal: 7458.62 Hz) (actual: 7458.50) (error: 0 cents)
  24440,
  // MIDI 119 - B8  (ideal: 7902.13 Hz) (actual: 7901.92) (error: 0 cents)
  25893,
  // MIDI 120 - C9  (ideal: 8372.02 Hz) (actual: 8371.89) (error: 0 cents)
  27433,
  // MIDI 121 - C#9 (ideal: 8869.84 Hz) (actual: 8869.63) (error: 0 cents)
  29064,
  // MIDI 122 - D9  (ideal: 9397.27 Hz) (actual: 9396.97) (error: 0 cents)
  30792,
  // MIDI 123 - D#9 (ideal: 9956.06 Hz) (actual: 9956.05) (error: 0 cents)
  32624,
  // MIDI 124 - E9  (ideal: 10548.08 Hz) (actual: 10547.79) (error: 0 cents)
  34563,
  // MIDI 125 - F9  (ideal: 11175.30 Hz) (actual: 11175.23) (error: 0 cents)
  36619,
  // MIDI 126 - F#9 (ideal: 11839.82 Hz) (actual: 11839.60) (error: 0 cents)
  38796,
  // MIDI 127 - G9  (ideal: 12543.85 Hz) (actual: 12543.64) (error: 0 cents)
  41103,
};
