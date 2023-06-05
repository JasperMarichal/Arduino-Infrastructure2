#include "display.h"
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>

/* Segment byte maps for numbers 0 to 9 */
const uint8_t SEGMENT_MAP[] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99,
                               0x92, 0x82, 0xF8, 0X80, 0X90};

/* Byte maps to select digit 1 to 4 */
const uint8_t SEGMENT_SELECT[] = {0xF1, 0xF2, 0xF4, 0xF8};

/* Segment byte maps for alphabet from a to z*/
const uint8_t ALPHABET_MAP[] = {0x88, 0x83, 0xC6, 0xA1, 0x86, 0x8E, 0xC2,
                                0x89, 0xCF, 0xE1, 0x8A, 0xC7, 0xEA, 0xC8,
                                0xC0, 0x8C, 0x4A, 0xCC, 0x92, 0x87, 0xC1,
                                0xC1, 0xD5, 0x89, 0x91, 0xA4};

/* Segment byte maps for symbols*/
const uint8_t SYMBOLS_MAP[] = {0x81, 0x42, 0x3C, 0x24, 0x18
                              , 0x99, 0x66, 0x5A, 0x87, 0x1E};

void initDisplay() {
  sbi(DDRD, LATCH_DIO);
  sbi(DDRD, CLK_DIO);
  sbi(DDRB, DATA_DIO);
}

// loop through seven segments of LED display and shift the correct bits in the
// data register
void shift(uint8_t val, uint8_t bitorder) {
  uint8_t bit;

  for (uint8_t i = 0; i < NUMBER_OF_SEGMENTS; i++) {
    if (bitorder == LSBFIRST) {
      bit = !!(val & (1 << i));
    } else {
      bit = !!(val & (1 << (7 - i)));
    }
    // write bit to register
    if (bit == HIGH)
      sbi(PORTB, DATA_DIO);
    else
      cbi(PORTB, DATA_DIO);

    // Trigger the clock pin so the display updates
    sbi(PORTD, CLK_DIO);
    cbi(PORTD, CLK_DIO);
  }
}

//Writes a digit to a certain segment. Segment 0 is the leftmost.
void writeNumberToSegment(uint8_t segment, uint8_t value) {
  cbi(PORTD, LATCH_DIO);
  shift(SEGMENT_MAP[value], MSBFIRST);
  shift(SEGMENT_SELECT[segment], MSBFIRST);
  sbi(PORTD, LATCH_DIO);
}

//Writes a nuber between 0 and 9999 to the display. To be used in a loop...
void writeNumber(int number) {
  if (number < 0 || number > 9999) return;
  writeNumberToSegment(0, number / 1000);
  writeNumberToSegment(1, (number / 100) % 10);
  writeNumberToSegment(2, (number / 10) % 10);
  writeNumberToSegment(3, number % 10);
}

//Writes a number between 0 and 9999 to the display and makes sure that it stays there a certain number of millisecs.
//Note: the timing is approximate; the amount of time writeNumberToSegment takes is not accounted for...
void writeNumberAndWait(int number, int delay) {
  if (number < 0 || number > 9999) return;
  for (int i = 0; i < delay / 20; i++) {
    writeNumberToSegment(0, number / 1000);
    _delay_ms(5);
    writeNumberToSegment(1, (number / 100) % 10);
    _delay_ms(5);
    writeNumberToSegment(2, (number / 10) % 10);
    _delay_ms(5);
    writeNumberToSegment(3, number % 10);
    _delay_ms(5);
  }
}

// Blanks a certain segment. Segment 0 is the leftmost.
void blankSegment(uint8_t segment)
{
  cbi(PORTD, LATCH_DIO);
  shift(0xFF, MSBFIRST);
  shift(SEGMENT_SELECT[segment], MSBFIRST);
  sbi(PORTD, LATCH_DIO);
}

void writeCharToSegment(uint8_t segment, char character) {
  if (character >= 'a' && character <= 'z') {
    character -= 32;
  }

  uint8_t value;
  if (character >= 'A' && character <= 'Z') {
    value = ALPHABET_MAP[character - 'A'];
  } else {
    value = 0xFF;
  }

  cbi(PORTD, LATCH_DIO);
  shift(value, MSBFIRST);
  shift(SEGMENT_SELECT[segment], MSBFIRST);
  sbi(PORTD, LATCH_DIO);
}

void writeString(char* str) {
  int length = strlen(str);
  int numLetters = length < 4 ? length : 4;

  for (int i = 0; i < numLetters; i++) {
    writeCharToSegment(i, str[i]);
  }
}

void writeStringAndWait(char* str, int delay) {
  for (int i = 0; i < delay / 20; i++) {
    writeCharToSegment(0, str[0]);
    _delay_ms(5);
    writeCharToSegment(1, str[1]);
    _delay_ms(5);
    writeCharToSegment(2, str[2]);
    _delay_ms(5);
    writeCharToSegment(3, str[3]);
    _delay_ms(5);
  }
}

void scrollText(char* str, int delay) {
  int textLength = strlen(str);

  for (int i = 0; i < textLength-3; i++) {
    char scrollText[5];
    for (int j = 0; j < 4; j++) {
      int index = (i + j) % textLength;
      scrollText[j] = str[index];
    }
    writeStringAndWait(scrollText, delay);
  }
}

void writeSymbolToSegment(uint8_t segment, uint8_t symbol) {
  cbi(PORTD, LATCH_DIO);
  shift(SYMBOLS_MAP[symbol], MSBFIRST);
  shift(SEGMENT_SELECT[segment], MSBFIRST);
  sbi(PORTD, LATCH_DIO);
}

void writeSymbolAndWait(uint8_t symbol, int delay) {
  for (int i = 0; i < delay / 20; i++) {
    writeSymbolToSegment(0, symbol / 1000);
    _delay_ms(5);
    writeSymbolToSegment(1, (symbol / 100) % 10);
    _delay_ms(5);
    writeSymbolToSegment(2, (symbol / 10) % 10);
    _delay_ms(5);
    writeSymbolToSegment(3, symbol % 10);
    _delay_ms(5);
  }
}