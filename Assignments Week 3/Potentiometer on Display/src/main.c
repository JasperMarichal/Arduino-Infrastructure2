#include <util/delay.h>
#include <avr/io.h>
#include <usart.h>
#include <display.h>

void initADC()
{
    ADMUX |= ( 1 << REFS0 );
    ADCSRA |= ( 1 << ADPS2 ) | ( 1 << ADPS1 ) | ( 1 << ADPS0 );
    ADCSRA |= ( 1 << ADEN );
}

int main()
{
    initUSART();
    initADC();
    while ( 1 )
    {
        ADCSRA |= ( 1 << ADSC );
        loop_until_bit_is_clear( ADCSRA, ADSC );
        uint16_t value = ADC;
        writeNumberAndWait(value, 10000);
        printf( "Value: %d\n", value );
    }
    return 0;
}