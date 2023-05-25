#include <util/delay.h>
#include <avr/io.h>
#include <usart.h>
#include <display.h>

void initADC()
{
    ADMUX |= ( 1 << REFS0 );
    ADMUX |= (1 << ADLAR);
    ADCSRA |= ( 1 << ADPS2 ) | ( 1 << ADPS1 ) | ( 1 << ADPS0 );
    ADCSRA |= ( 1 << ADEN );
}

int main()
{
    initUSART();
    initADC();
    initDisplay();

    while ( 1 )
    {
        for (uint8_t channel = 0; channel < 3; channel++)
        {
            ADMUX = (ADMUX & 0xF0) | (channel + 1);  // Set ADC input channel PC1, PC2, PC3

            ADCSRA |= (1 << ADSC);  // Start ADC conversion
            loop_until_bit_is_clear(ADCSRA, ADSC);  // Wait for conversion to complete

            uint8_t value = ADCH;  // Read the ADC result (8-bit mode)
            writeNumberAndWait(value, 500);
            printf("Value: %u\n", value);
        }
    }
    return 0;
}