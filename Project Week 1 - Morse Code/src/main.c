#define TABLE_LENGTH 36
#define SHORT_BLINK 100
#define LONG_BLINK 1000
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <util/delay.h>
#include <ledlib.h>
#include <usart.h>

typedef struct Morse {
    char ch;
    const char *code;
} Morse;

char *getCode(char ch) 
{
    switch(ch) {

        case 'A': return ".-";
        case 'B': return "-...";
        case 'C': return "-.-.";
        case 'D': return "-..";
        case 'E': return ".";
        case 'F': return "..-.";
        case 'G': return "--.";
        case 'H': return "....";
        case 'I': return "..";
        case 'J': return ".---";
        case 'K': return "-.-";
        case 'L': return ".-..";
        case 'M': return "--";
        case 'N': return "-.";
        case 'O': return "---";
        case 'P': return ".--.";
        case 'Q': return "--.-";
        case 'R': return ".-.";
        case 'S': return "...";
        case 'T': return "-";
        case 'U': return "..-";
        case 'V': return "...-";
        case 'W': return ".--";
        case 'X': return "-..-";
        case 'Y': return "-.--";
        case 'Z': return "--..";
        case '0': return "-----";
        case '1': return ".----";
        case '2': return "..---";
        case '3': return "...--";
        case '4': return "....-";
        case '5': return ".....";
        case '6': return "-....";
        case '7': return "--...";
        case '8': return "---..";
        default: return "----."; //aka this is 9
    }
}

void fillUpTable(Morse *table) 
{
    int next_letter = 65;
    int next_number = 48;
    const int endOfLetters = 25;

    for (int i = 0; i < TABLE_LENGTH; i++) {
        
        if (i <= endOfLetters) {
            table[i].ch = next_letter++;
        } else {
            table[i].ch = next_number++;
        }

        table[i].code = getCode(table[i].ch);
    }
}

void countDownPattern() 
{
    _delay_ms(500);

    for (int i = 0; i < 4; i++) {
        lightDownOneLed(i);
        _delay_ms(500);
    }
}

int getRandomIndex() 
{
    return rand() % 36;
}

void blinkCode(const char *code) 
{
    for (int i = 0; i < strlen(code); i++) {

        lightUpAllLeds();

        if (code[i] == '.') {
            _delay_ms(SHORT_BLINK);
        } else {
            _delay_ms(LONG_BLINK);
        }

        lightDownAllLeds();
        _delay_ms(500);
    }
}

void frivolousLedDance() 
{
    ledChaos();
}

int main() { //Morse trainer

    initUSART();
    enableAllLeds();

    Morse table[TABLE_LENGTH];
    fillUpTable(table);
    
    countDownPattern();

    printf("\n==== MORSE TRAINER ====\n");

    for (int i = 0; i < 10; i++) {
        Morse *random = &table[getRandomIndex(table)];
        blinkCode(random->code);
        _delay_ms(1000);
        printf("[%d] Solution: %c (%s)\n", i + 1, random->ch, random->code);
        _delay_ms(1000);
    }

    printf("\n==== END OF MORSE TRAINER ====");
    frivolousLedDance();

    return 0;
}