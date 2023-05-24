#include <util/delay.h>
#include <avr/io.h>
#include <usart.h>
#include <display.h>
#define number1 2349
#define number2 7830
#define number3 1974


int main()
{
    initUSART();
    initDisplay();
    while ( 1 )
    {
        for ( int i = 0; i < 4; i++ )
        {
            writeNumberToSegment( i, 8 );
            _delay_ms( 1000 );
        }
        /* Use the writeNumber function if
         * you want to display a 4 digit number. */
        writeNumber( number1 );
        _delay_ms( 1000 );
        /* Problem: this function shows the digits 1 by 1 and then
         * adds a delay. So, only the last digit is displayed. 
         * Solution: to display the complete number, repeat this in a loop. */
        for ( int i = 0; i < 10000; i ++ )
        {
            writeNumber( number2 );
        }
        /* Or you could use a function from the library.
         * Then you can also specify the display duration. */
        writeNumberAndWait( number3, 10000 );
    }
    return 0;
}