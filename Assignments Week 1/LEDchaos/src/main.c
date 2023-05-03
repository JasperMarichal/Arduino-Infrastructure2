#define __DELAY_BACKWARD_COMPATIBLE__
#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>
#include <ledlib.h>

void main(int iterations) {

    srand(0);
    enableAllLeds();
    lightDownAllLeds();

    for (int i = 1; i <= iterations; i++) {
        /* TODO led 2 and 3 blink at the beginning, then stay turned on */
        uint8_t leds = rand() % 4;
        leds += 2;
        //printf("%d\n", leds);
        lightUpAllLeds();
        _delay_ms(rangernd(100, 1000));
        //wrapper_delay_ms(rangernd(100, 1000));
        lightDownAllLeds();
        _delay_ms(100);
    }
}

int rangernd(int low, int up) {
    return rand() % (up + 1 - low) + low;
}