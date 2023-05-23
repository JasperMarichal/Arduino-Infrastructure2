#include <stdint.h>

void enableOneLed ( int lednumber );
// The enableOneLed() function takes an integer argument lednumber, which corresponds to the number of the LED to be enabled. If lednumber is out of range, i.e., less than 0 or greater than NUMBER_OF_LEDS-1, the function returns without doing anything. Otherwise, the function sets the direction of the corresponding I/O pin to output, which enables the LED.

void enableMultipleLeds (uint8_t leds);
// The enableMultipleLeds() function takes an 8-bit unsigned integer argument leds, which is a bitmask indicating which LEDs to enable. The function iterates over all the LEDs and calls the enableOneLed() function for each LED that is enabled in the bitmask.

void enableAllLeds (void);

void lightUpOneLed ( int lednumber );
// The lightUpOneLed() function turns on the LED specified by lednumber. If lednumber is out of range, the function returns without doing anything. Otherwise, the function clears the corresponding bit of the PORTB register, which turns on the LED.

void lightUpMultipleLeds (uint8_t leds);
// The lightUpMultipleLeds() function takes an 8-bit unsigned integer argument leds, which is a bitmask indicating which LEDs to turn on. The function iterates over all the LEDs and calls the lightUpOneLed() function for each LED that is turned on in the bitmask.

void lightUpAllLeds (void);

void lightDownOneLed ( int lednumber );
// The lightDownOneLed() function turns off the LED specified by lednumber. If lednumber is out of range, the function returns without doing anything. Otherwise, the function sets the corresponding bit of the PORTB register, which turns off the LED.

void lightDownMultipleLeds (uint8_t leds);
// The lightDownMultipleLeds() function takes an 8-bit unsigned integer argument leds, which is a bitmask indicating which LEDs to turn off. The function iterates over all the LEDs and calls the lightDownOneLed() function for each LED that is turned off in the bitmask.

void lightDownAllLeds (void);
// The enableAllLeds(), lightUpAllLeds(), and lightDownAllLeds() functions are convenience functions that call the respective multiple LED functions with a bitmask representing all the LEDs.

void lightToggleOneLed ( int lednumber );
// The lightToggleLed() function toggles the state of the LED specified by lednumber. If lednumber is out of range, the function returns without doing anything. Otherwise, the function flips the state of the corresponding bit of the PORTB register, which toggles the LED.

void dimLed(int lednumber, int percentage, int duration);

void fadeInLed(int lednumber, int duration);

void fadeOutLed(int lednumber, int duration);

void blinkLed(int lednumber, int duration);
// Made for Basic C exercise two

void flashLed(int led, int flashes); 
// This is the function of Basic C exersice three

void ledChaos();
/* Overall, this code provides a set of functions for enabling, turning on/off, toggling, and manipulating multiple LEDs using a bitmask.*/