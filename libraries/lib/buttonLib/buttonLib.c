#include <util/delay.h>
#include <avr/io.h>
#include<avr/sfr_defs.h>
#include<usart.h>
#include <avr/interrupt.h>

#define NUMBER_OF_BUTTONS 3
void enableButton(int button)
{
    if ( button < 0 || button > NUMBER_OF_BUTTONS-1 ) return;
    DDRC &= ~_BV( PC1+button ); /* Button 1 is connected to C1.
                             * We set the corresponding bit in the Data Direction Register to 0. */
    PORTC |= _BV( PC1+button );  /* Activate the Pull-up of C1: PINC will be set to high */
   
}

void initButtons()
{
    enableButton(0); 
    enableButton(1); 
    enableButton(2); 

    PCICR |= (1 << PCIE1); 
    PCMSK1 |= (1 << PCINT9) | (1 << PCINT10) | (1 << PCINT11);

    sei();
}

int buttonPushed(int button)
{
    if ( button < 0 || button > NUMBER_OF_BUTTONS-1 ) return -1;
   if ( bit_is_clear( PINC, (PC1+button) )) /* instead of (!(PINC & ( 1 << PC1 ))) we use the macro bit_is_clear */
        {
            return 1;
        }
    return 0;
}

int buttonReleased(int button)
{
    PORTC &= _BV( PC1+button );
    if ( button < 0 || button > NUMBER_OF_BUTTONS-1 ) return -1;
    if (!( bit_is_clear( PINC, (PC1+button) ))) /* instead of (!(PINC & ( 1 << PC1 ))) we use the macro bit_is_clear */
        {
           return 1;
        }
    return 0;
}

int readButton()
{
    if (bit_is_clear(PINC, PC1))
    {
        return 0; // Button 1 pressed
    }
    else if (bit_is_clear(PINC, PC2))
    {
        return 1; // Button 2 pressed
    }
    else if (bit_is_clear(PINC, PC3))
    {
        return 2; // Button 3 pressed
    }
    else
    {
        return -1; // No button pressed
    }
}