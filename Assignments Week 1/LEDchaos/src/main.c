#define __DELAY_BACKWARD_COMPATIBLE__
#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>
#include <ledlib.h>

int main() {
    srand(0);
    enableAllLeds();
    lightDownAllLeds();

    while(1){
        int led = rand() % 4;
        lightUpOneLed(led);
        _delay_ms(500);
        lightDownOneLed(led);
    }
    return 0;
}
