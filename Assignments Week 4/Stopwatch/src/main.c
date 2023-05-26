#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include <display.h>
#include <usart.h>

// Global variables
// Enter here your global variables
uint32_t counter = 0;
#define MULTIPLE 250

void initTimer() {    
    TCCR0A |= _BV( WGM21 );

    OCR2A = 249;

    TIMSK2 |= _BV( OCIE0A );
    sei();  
}

void startTimer() {  
    TCCR2B |= _BV( CS22 ) | _BV(CS21);
}

void stopTimer() {    
    TCCR2B &= ~(_BV( CS22 ) | _BV(CS21) | _BV(CS20));
}

void tick() {    
    // enter your code
}

void writeTimeAndWait(uint8_t minuten, uint8_t seconden, int delay) {    
    // enter your code
}

ISR(TIMER2_COMPA_vect) {
    counter++;
    if((counter+1)%MULTIPLE == 0) {
        counter=0;
    }
}

int main() {    
    initUSART();    
    initDisplay();

    //Initilaize timer
    printf("Start the stopwatch by pressing button S1, stop by pressing button S2, and reset with S3\n");    
    while (1) {        
        
    }
    return 0;
}
