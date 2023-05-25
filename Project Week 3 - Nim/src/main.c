#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <display.h>
#include <usart.h>
#include <util/delay.h>
#include <buttonlib.h>

#define MIN_TAKE 1
#define MAX_TAKE 3
#define START_NUMBER 21
#define BUTTON_DELAY 500

int main() { //nim
    int moves_len = 10;
    int *moves = malloc(moves_len * sizeof(int));
    int seed;
    int matches_left = START_NUMBER;
    bool player_next;
    bool player_starts;

    initUSART();
    initDisplay();
    initADC_nim();

    printf("\n\n============== NIM ==============");
    printf("\nTurn the potentiometer to seed rand()!");
    _delay_ms(5000);
    seed = getseed();
    srand(seed);
    player_next = seed % 2;
    player_starts = player_next;

    printf("\nSeed: %d", seed);
    printf("\nPress button 1 to proceed!");

    while (!buttonPushed(0)) { 
        writeNumber(0);
    }

    for (int i = 0; matches_left > 0; i++) {

        int take = MIN_TAKE;

        printf("\nTake %d: %s", i + 1, player_next ? "you!" : "arduino");
       
        while (true) {

            lightUpSegments(take, matches_left, player_next);

            if (player_next) {
                if (buttonPushed(0) && take > MIN_TAKE) {
                    _delay_ms(BUTTON_DELAY);
                    take--;
                } else if (buttonPushed(1)) {
                    _delay_ms(BUTTON_DELAY);
                    break;
                } else if (buttonPushed(2) && take < MAX_TAKE) {
                    _delay_ms(BUTTON_DELAY);
                    take++;
                }

            } else {                
                take = (matches_left - 1) % (MAX_TAKE + 1);

                if (take == 0)
                    take = (rand() % MAX_TAKE) + 1;

                printf("\nArduino takes %d. Press button 2 to confirm.", take);

                while(!buttonPushed(1)) { 
                    lightUpSegments(take, matches_left, player_next);
                };  

                _delay_ms(BUTTON_DELAY);  

                break;
            }
            
        }

        matches_left -= take;
        player_next = !player_next;

        if (i == moves_len - 1) {
            moves_len += 5;
            moves = realloc(moves, moves_len);
            
            if (!moves) {
                free(moves);
                break;
            }
        }
        moves[i] = take;
    }

    printf("\n%s", player_next ? "YOU WON!" : "YOU LOST!");
    printf("\nPress button 1 to see a breakdown.");

    while(!buttonPushed(0)) {
        writeString(player_next ? "PPPP" : "CCCC");
    }

    stats(moves, moves_len, player_starts);
    free(moves);
    printf("\n\n============== END ==============");
    return 0;
}


void initADC_nim(void)
{
    ADMUX |= _BV(REFS0);
    ADCSRA |= _BV(ADPS2) | _BV(ADPS1);
    ADCSRA |= _BV(ADEN);
}

int getseed(void)
{
    _delay_ms(5000);
    ADCSRA |= _BV(ADSC);
    loop_until_bit_is_clear(ADCSRA, ADSC);    
    return ADC;
}

void stats(int *moves, int moves_len, bool player_next)
{
    int left = START_NUMBER;
    for (int i = 0; i < moves_len; i++, left -= moves[i], player_next = !player_next) {
        printf("\nMove %d: %s took %d. Matches left: %d",
            i, player_next ? "you" : "arduino", moves[i], left);
    }
}

void lightUpSegments(int take, int matches_left, bool player_next)
{
    writeNumberToSegment(0, take);
    writeCharToSegment(1, player_next ? 'P' : 'C');
    writeNumberToSegment(2, matches_left / 10);
    writeNumberToSegment(3, matches_left % 10);
}