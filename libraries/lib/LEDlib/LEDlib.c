// to allow variables as parameter for the _delay-functions (must be placed before the include of delay.h)
#define __DELAY_BACKWARD_COMPATIBLE__
#include <util/delay.h>
#include <avr/io.h>
#include "LEDlib.h"

//The next line defines a constant called NUMBER_OF_LEDS and assigns it the value of 4. This constant is used throughout the code to ensure that only the specified number of LEDs are manipulated.
#define NUMBER_OF_LEDS 4 //Define is a "preprocessor directive". It ensures that every NUMBER_OF_LEDS will be replaced by 4 in the following code

void enableOneLed ( int lednumber ) //C has no classes; functions can be included directly in the .c file.
{
    if ( lednumber < 0 || lednumber > NUMBER_OF_LEDS-1 ) return;
    DDRB |= ( 1 << ( PB2 + lednumber ));    
}

void enableMultipleLeds (uint8_t leds)
{
    for ( int i = 0; i < NUMBER_OF_LEDS; i++ )
    {
        if ( leds & ( 1 << i )) enableOneLed ( i );
    }
}

void enableAllLeds (void)
{
    enableMultipleLeds ( 0b1111 );
}

void lightUpOneLed ( int lednumber )    
{
    if ( lednumber < 0 || lednumber > NUMBER_OF_LEDS-1 ) return;
    PORTB &= ~( 1 << ( PB2 + lednumber ));  
}

void lightUpMultipleLeds (uint8_t leds)
{
    for ( int i = 0; i < NUMBER_OF_LEDS; i++ )
    {
        if ( leds & ( 1 << i )) lightUpOneLed ( i );
    }
}

void lightUpAllLeds (void)
{
    lightUpMultipleLeds ( 0b1111 );
}

void lightDownOneLed ( int lednumber )
{
    if ( lednumber < 0 || lednumber > 3 ) return;
    PORTB |= ( 1 << ( PB2 + lednumber ));   
}

void lightDownMultipleLeds (uint8_t leds)
{
    for ( int i = 0; i < NUMBER_OF_LEDS; i++ )
    {
        if ( leds & ( 1 << i )) lightDownOneLed ( i );
    }
}

void lightDownAllLeds (void)
{
    lightDownMultipleLeds ( 0b1111 );
}

void lightToggleOneLed ( int lednumber )
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
    lightUpOneLed(lednumber);
    _delay_ms(onTime);
    lightDownOneLed(lednumber);
    _delay_ms(offTime);
}

void fadeInLed(int lednumber, int duration)
{
    for (int i = 0; i < 100; i++)
    {
        dimLed(lednumber, i, duration);
    }
}

void fadeOutLed(int lednumber, int duration)
{
    for (int i = 100; i > 0; i--)
    {
        dimLed(lednumber, i, duration);
    }
}

void blinkLed(int lednumber, int duration) 
{
    lightUpOneLed(lednumber);
    _delay_ms(duration);
    lightDownOneLed(lednumber);
    _delay_ms(duration);
}

void flashLed(int led, int flashes) 
{
    enableOneLed(led);

    for (int i = 0; i < flashes; i++) {
        lightUpOneLed(led);
        _delay_ms(500);
        lightDownOneLed(led);
    }
}

void ledChaos(){
    srand(0);
    enableAllLeds();
    lightDownAllLeds();

    for (int i = 0; i < 100; i++)
    {
        int led = rand() % 4;
        lightUpOneLed(led);
        _delay_ms(100);
        lightDownOneLed(led);
    }
}