#include <util/delay.h>
#include <avr/io.h>
#include <usart.h>

void initADC()
{
    ADMUX |= (1 << REFS0);                                // Set up the reference voltage. We choose 5V as the reference.
    ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Determine the sample rate by setting the division factor to 128.
    ADCSRA |= (1 << ADEN);                                // Enable the ADC
    ADCSRA |= (1 << ADATE);                               // Enable ADC Auto Triggering
    ADCSRB = 0;                                                // Set ADC Auto Trigger Source to Free Running Mode (default)
    ADCSRA |= (1 << ADSC);                                // Start the analog-to-digital conversion
}

int main()
{
    initUSART();
    initADC();
    uint16_t value = 0;

    while (1)
    {
        value = ADC; // Read the result immediately
        printf("Value: %d\n", value);
        _delay_ms(100); // Delay for better readability, adjust as needed
    }

    return 0;
}