#include <stdio.h>
#include <string.h>
#include <util/delay.h>
#include <ledlib.h>

int main(){
char line[] = "aabdcbcadcbadcbdab";
task8(line);

  return 0;
}

void task1(void) {
    while(1) {
    for (int i = 0; i < 4; i++) {
        enableOneLed(i);
        lightUpOneLed(i);
        _delay_ms(500);
    }
    lightDownAllLeds();
    _delay_ms(100);
    }
}

void task2(){
    int led = 0;
    enableOneLed(led);
    
    for (int i = 0; i < 10; i++) {
        lightUpOneLed(led);
        _delay_ms(500);
        lightDownOneLed(led);
    }
}

void task3(int led, int flashes) //Function LEDlib
{
    enableOneLed(led);
    for (int i = 0; i < flashes; i++) {
        lightUpOneLed(led);
        _delay_ms(500);
        lightDownOneLed(led);
    }
}

void task4(int led) 
{
    enableOneLed(led);

    for (int i = 10; i <= 1000; i += 50) {
        lightUpOneLed(led);
        _delay_ms(i);
        lightDownOneLed(led);
    }
}

void task5(int arr[10]) 
{
    enableAllLeds();

    for (int i = 0; i < 10; i++) {
        int led = arr[i];
        lightUpOneLed(led);
        _delay_ms(1000);
        lightDownOneLed(led);
    }
}

void task6(void) 
{
    const int length = 1000 / 50;
    int arr[length];
    int value = 10, led = 0;

    enableOneLed(led);

    for (int i = 0; i < length; i++, value += 50) {
        arr[i] = value;
        blinkLed(led, arr[i]);
    }    
}

void task7(char line[]) 
{
    const int ascii_offset = 97;

    for (int i = 0; i < strlen(line); i++) {
        if (line[i] >= 'a' && line[i] <= 'd') {
            int led = line[i] - ascii_offset;
            enableOneLed(led);
            lightUpOneLed(led);
            _delay_ms(1000);
            lightDownOneLed(led);
        }        
    }
}

void task8(char line[]) 
{
    const int ascii_offset = 97;
    enableAllLeds();

    for (int i = 0; i < strlen(line); i++) {

        if (line[i] >= 'a' && line[i] <= 'd') {
            switch(line[i]) {
                case 'a':
                    lightUpAllLeds();
                    _delay_ms(100);
                    lightDownAllLeds();
                    break;
                case 'b':
                    for (int i = 0; i < 2; i++) {
                    lightUpAllLeds();
                    _delay_ms(100);
                    lightDownAllLeds();
                    _delay_ms(100);
                    }       
                    break;
                case 'c':
                    for (int i = 0; i < 3; i++) {
                        lightUpAllLeds();
                        _delay_ms(100);
                        lightDownAllLeds();
                        _delay_ms(100);
                    }
                    break;
                case 'd':
                    for (int i = 0; i < 4; i++) {
                        lightUpAllLeds();
                        _delay_ms(100);
                        lightDownAllLeds();
                        _delay_ms(100);
                    }
                    break;
            }            
        }
        _delay_ms(1000);
    }
}