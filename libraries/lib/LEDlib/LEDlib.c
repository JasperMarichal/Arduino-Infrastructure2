// to allow variables as parameter for the _delay-functions (must be placed before the include of delay.h)
#define __DELAY_BACKWARD_COMPATIBLE__
#include <util/delay.h>
#include <avr/io.h>
#include "LEDlib.h"

//The next line defines a constant called NUMBER_OF_LEDS and assigns it the value of 4. This constant is used throughout the code to ensure that only the specified number of LEDs are manipulated.
#define NUMBER_OF_LEDS 4 //Define is a "preprocessor directive". It ensures that every NUMBER_OF_LEDS will be replaced by 4 in the following code

void enableLed ( int lednumber ) //C has no classes; functions can be included directly in the .c file.
{
    if ( lednumber < 0 || lednumber > NUMBER_OF_LEDS-1 ) return;
    DDRB |= ( 1 << ( PB2 + lednumber ));    //Check the tutorial "Writing to a Pin". We know from the documentation on
                                            //the multifunctional shield that the LEDs start at PB2
}

void enableMultipleLeds (uint8_t leds)
{
    for ( int i = 0; i < NUMBER_OF_LEDS; i++ )
    {
        if ( leds & ( 1 << i )) enableLed ( i );
    }
}

void enableAllLeds (void)
{
    enableMultipleLeds ( 0b1111 );
}

void lightUpLed ( int lednumber )    //Note: enabled LEDs light up immediately ( 0 = on )
{
    if ( lednumber < 0 || lednumber > NUMBER_OF_LEDS-1 ) return;
    PORTB &= ~( 1 << ( PB2 + lednumber ));  //Check the tutorial on "Bit Operations" to know what happens in this line.
}

void lightUpMultipleLeds (uint8_t leds)
{
    for ( int i = 0; i < NUMBER_OF_LEDS; i++ )
    {
        if ( leds & ( 1 << i )) lightUpLed ( i );
    }
}

void lightUpAllLeds (void)
{
    lightUpMultipleLeds ( 0b1111 );
}

void lightDownLed ( int lednumber )
{
    if ( lednumber < 0 || lednumber > 3 ) return;
    PORTB |= ( 1 << ( PB2 + lednumber ));   //Make sure you understand this line as well!
}

void lightDownMultipleLeds (uint8_t leds)
{
    for ( int i = 0; i < NUMBER_OF_LEDS; i++ )
    {
        if ( leds & ( 1 << i )) lightDownLed ( i );
    }
}

void lightDownAllLeds (void)
{
    lightDownMultipleLeds ( 0b1111 );
}

void lightToggleLed ( int lednumber )
{
    if ( lednumber < 0 || lednumber > NUMBER_OF_LEDS-1 ) return;
    PORTB ^= ( 1 << ( PB2 + lednumber ));
}

void dimLed(int lednumber, int percentage, int duration)
{
    if (lednumber < 0 || lednumber > NUMBER_OF_LEDS-1)
    {
        return;
    }
    if (percentage < 0 || percentage > 100)
    {
        return;
    }
    if (duration < 0)
    {
        return;
    }
    int onTime = (duration * percentage) / 100;
    int offTime = duration - onTime;
    lightUpLed(lednumber);
    _delay_ms(onTime);
    lightDownLed(lednumber);
    _delay_ms(offTime);
}

void fadeLed(int lednumber, int duration)
{
    for (int i = 0; i < 100; i++)
    {
        dimLed(lednumber, i, duration);
    }
}

void blinkLed(int lednumber, int duration) 
{
    lightUpLed(lednumber);
    _delay_ms(duration);
    lightDownLed(lednumber);
    _delay_ms(duration);
}

void flashLed(int led, int flashes) 
{
    enableLed(led);

    for (int i = 0; i < flashes; i++) {
        blinkLed(led, 100);
    }
}