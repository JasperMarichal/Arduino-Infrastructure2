#define Delay 500
#include <stdbool.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <usart.h>
#include <ledlib.h>
#include <buttonlib.h>

volatile int button_pushed = 0;
volatile int counter = 0;

ISR(PCINT1_vect) {
    button_pushed = 1;
}

void generatePuzzle(uint8_t* puzzle, int length, int seed) {
    srand(seed);

    for(int i = 0; i < length; i++) {
        puzzle[i] = rand() % 3;
    }
}

void printPuzzle(uint8_t* puzzle, int length) {
    printf("[ ");
    for(int i = 0; i < length; i++) {
        printf("%d ", puzzle[i] + 1);
    }
    printf("]\n");
}

void playPuzzle(uint8_t* puzzle, int length) {
    for (int i = 0; i < length; i++) {
        switch (puzzle[i]) {
            case 1:
                lightUpOneLed(0);
                _delay_ms(Delay);
                lightDownOneLed(0);
                break;
            case 2:
                lightUpOneLed(1);
                _delay_ms(Delay);
                lightDownOneLed(1);
                break;
            case 3:
                lightUpOneLed(2);
                _delay_ms(Delay);
                lightDownOneLed(2);
                break;
        }
        _delay_ms(Delay);
    }
}

bool readInput(uint8_t* puzzle, int length) {
    int userInput[length];
    int inputIndex = 0;

    while (inputIndex < length) {
        if (button_pushed) {
            button_pushed = 0;
        
        int buttonIndex = readButton();
        userInput[inputIndex] = buttonIndex;

        printf("You pressed button %d\n", buttonIndex + 1);
        if (buttonIndex + 1 != puzzle[inputIndex]) {
            printf("Wrong!\n");
            printf("The correct sequence was: ");
            printPuzzle(puzzle, length);
            return false;
        } else {
            printf("Correct!\n");
        }

        inputIndex++;
        _delay_ms(100);
        }
    }
    printf("Correct, we go to level %d!\n", length + 1);
    return true;
}

int main() { //Simon says function
    enableAllLeds();
    lightDownAllLeds();
    initButtons();
    initUSART();
    int level = 1;
    uint8_t puzzle[level];
    
    printf("=== Simon says ===\n");
    flashLed(3, 7);
    printf("Press button 1 to start level %d!\n", level);

while (true) {
        if (button_pushed) {
            button_pushed = 0;
            break;
        }
        lightToggleOneLed(3);
        _delay_ms(100);
        counter++;
    }

    lightDownAllLeds();
    printf("Game started!\n");
    generatePuzzle(puzzle, level, counter);
    printPuzzle(puzzle, level);

    while (level <= 10) {
        printf("=== Level %d ===\n", level);
        playPuzzle(puzzle, level);

        if (readInput(puzzle, level)) {
            level++;
            generatePuzzle(puzzle, level, counter);
            printPuzzle(puzzle, level);
        } else {
            break;
        }
    }
    if (level == 11) {
        printf("Congratulations, you are the Simon Master!\n");
    }
    return 0;
}