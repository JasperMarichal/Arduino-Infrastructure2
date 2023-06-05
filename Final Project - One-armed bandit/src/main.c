#define __DELAY_BACKWARD_COMPATIBLE__
#include <util/delay.h>
#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <usart.h>
#include <buttonLib.h>
#include <LEDlib.h>
#include <display.h>
#include <avr/interrupt.h>

typedef struct {
    int coins_left;
    int betAmount;
    int finalNumber;
    int winAmount;
    int totalWins;
} GameProgress;

//Define variables
#define COINS 50
#define BUTTON_DELAY 500
//While loop randomGenerator
volatile uint32_t elapsedSeconds = 0;
volatile int status = 0;
int secondsCounter = 0;
volatile GameProgress gameProgress = {.coins_left = COINS};

//Random number generator methods
int generateRandomSymbol(int min, int max){
    return (rand() % (max - min + 1)) + min;
}

void initADC(void)
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

//Timer to count elapsed seconds
//TODO: Modify it for better code understanding
void initTimer() {
    TCCR0A |= _BV(WGM01);
    TCCR0B |= _BV(CS01) | _BV(CS00);
    OCR0A = 249;
    TIMSK0 |= _BV(OCIE0A);
}

ISR(TIMER0_COMPA_vect) {
    if(elapsedSeconds == 1200 ){
        secondsCounter++;
        elapsedSeconds = 0;
    }
    elapsedSeconds++;
}

//Centre button interrupt to show amount of coins left
ISR(PCINT1_vect) {
    if(buttonPushed(1) && status == 1){ //This will show the amount of coins left
        _delay_ms(BUTTON_DELAY);
        while(!buttonPushed(1)){ //When button is clicked again it will go back to the game
            writeNumber(gameProgress.coins_left);
        }
        _delay_ms(BUTTON_DELAY);
        return;
    } else if (buttonPushed(1) && status == 0){
        _delay_ms(BUTTON_DELAY);
        status = 1;
        return;
    }
}

void initButton(){
  enableButton(0);
  enableButton(1);
  enableButton(2);
  PCICR |= (1 << PCIE1);
  PCMSK1 |= (1 << PCINT9) | (1 << PCINT10) | (1 << PCINT11);
  sei();
}

//LightUpSegments methods
void lightUpSegmentsRandomSymbol(int slotSymbols[], int numSlots){
    for (int i = 0; i < numSlots; i++) {
        writeSymbolToSegment(i, slotSymbols[i]);  
        _delay_ms(50);          
    }
}

void lightUpSegmentsLEDdisplayChoice(int slotAmount, int betAmount){
    writeCharToSegment(0, 'S');
    writeNumberToSegment(1, slotAmount);
    writeCharToSegment(2, 'C');
    writeNumberToSegment(3, betAmount);
}

//Light up leds when less than 4 coins are left
void remainingCoins(int coins_left){
    if(coins_left == 4){
        lightUpAllLeds();
    } else if (coins_left == 3){
        lightUpMultipleLeds(0b00000111);
    } else if (coins_left == 2) {
        lightUpMultipleLeds(0b00000011);
    } else if (coins_left == 1) {
        lightUpMultipleLeds(0b00000001);
    }
}

void enableBuzzer()
{
   DDRD |= ( 1 << PD3 );
}

void playTone( float frequency, uint32_t duration )
{
    uint32_t periodInMicro = ( uint32_t ) ( 1000000 / frequency ); //Calculate the period in microsecs from the freq
   uint32_t durationInMicro = duration * 1000; //We express duration in microsecs
   for ( uint32_t time = 0; time < durationInMicro; time += periodInMicro ) //See tutorial on Music for more info!
    {
    PORTD &= ~( 1 << PD3 ); //turn the buzzer on
    _delay_us( periodInMicro / 2 ); //Wait for the half of the period
    PORTD |= ( 1 << PD3 ); //Turn the buzzer off
    _delay_us( periodInMicro / 2 ); //Wait again for half of the period
    }
}

void playVictorySound(){
    enableBuzzer();
    playTone(500, 100);
    _delay_ms(200);
    playTone(700,150);
    _delay_ms(200);
    playTone(900,200);
}

void playMoneyFallingSound(){
    enableBuzzer();
    for (int i =0; i < 10; i++){
        float frequency = generateRandomSymbol(1000, 3000);
        uint32_t duration = generateRandomSymbol(50, 150);
        playTone(frequency, duration);
        _delay_ms(100);
    }
}

void playEndSound(){
    enableBuzzer();
    playTone(1000, 200);
    _delay_ms(200);
    playTone(800, 200);
    _delay_ms(200);
    playTone(600, 200);
    _delay_ms(200);
    playTone(400, 200);
    _delay_ms(200);
    playTone(200, 200);
}

void updateGameProgress(int betAmount, int finalNumber, int winAmount){
    gameProgress.betAmount = betAmount;
    gameProgress.finalNumber = finalNumber;
    gameProgress.winAmount = winAmount;
}

void checkWinCondition(int numSlots, int betAmount, int finalNumber){
    //Check if the player has won
    int check = 0;
    int number = finalNumber;
    for (int i = 1; i < numSlots; i++) {
        if (number % 10 == (number / 10) % 10) {
        check++;
        }
        number /= 10;
    }

    //If won add coins
    int winAmount = 0;
    if(numSlots-1 == check){
    //Victory sound
    printf("You won!\n");
        playVictorySound();
        if(numSlots == 2){
            winAmount = 5;
        } else if (numSlots == 3){
            winAmount = 50;
        } else if (numSlots == 4){
            winAmount = 500;
        }
        winAmount *= betAmount;
        printf("You won %d coins!\n", winAmount);
        gameProgress.coins_left += winAmount;
        writeNumberAndWait(winAmount, 2000);
        gameProgress.totalWins++;
        }

    updateGameProgress(betAmount, finalNumber, winAmount);
}

void playSlotMachine(int numSlots, int betAmount){
     // Slot machine
        int* slotNumbers = (int*)malloc(numSlots * sizeof(int));
        int* slotsActive = (int*)malloc(numSlots * sizeof(int));
        int tenSeconds = 0;

        for (int i = 0; i < numSlots; i++) {
            if (slotsActive[i]==0) {
                slotsActive[i] = 1;
            }
        }

        while(tenSeconds < 75) {
            for (int i = 0; i < numSlots; i++) {
                if (slotsActive[i]) {
                    // Change the number in the slot if it is still active
                    slotNumbers[i] = generateRandomSymbol(0, 9);
                    }
                    if (tenSeconds == 50 + (5*i)) {            
                        // Check when slot needs to be locked and can't change anymore
                        slotsActive[i] = 0;
                    }
                }
                lightUpSegmentsRandomSymbol(slotNumbers, numSlots);
                tenSeconds++;
            }
        tenSeconds = 0;

        //The final number will be showed one by one with a pause
        int finalNumber = 0;
        for (int i = 0; i < numSlots; i++) {
            finalNumber = finalNumber * 10 + slotNumbers[i];
            writeSymbolToSegment(i, slotNumbers[i]);
            _delay_ms(500);
        }
        //Free memory allocation
        free(slotNumbers);
        free(slotsActive);

        //Click sound when final number is shown
        playTone(1000,50);
        //The final number will be shown in one time
        printf("The final number is: %d\n", finalNumber);
        writeSymbolAndWait(finalNumber, 2000);

        checkWinCondition(numSlots, betAmount, finalNumber);
}

void startGame() {
    int betPlaced = 0;
    int betMAX = 4;
    int numSlots = 2;
    int betAmount = 1;

    printf("\n\nPress button 0 to change the number of slots and the bet amount\n");
    printf("Display slots = %d and bet amount = %d\n", numSlots, betAmount);
    printf("Press button 2 to place your bet\n");

    while (1) { //Chose how many slots you wanna play + putting coin in
        lightUpSegmentsLEDdisplayChoice(numSlots, betAmount);
        if (buttonPushed(0)) { //Choose number of slots and bet amount
        _delay_ms(BUTTON_DELAY);
        if(!betPlaced){ //Choose number of slots if bet is not placed yet
        numSlots = (numSlots % 3) + 2;
        printf("Number of display slots: %d\n", numSlots);
        } else { // Choose bet amount if bet is placed
            if(gameProgress.coins_left < 4) {
            betMAX = gameProgress.coins_left;
            }
        betAmount = (betAmount % betMAX) + 1;
        printf("Bet amount: %d\n", betAmount);
        }
    } 
        if (buttonPushed(2)) { //Placing bet
        _delay_ms(BUTTON_DELAY);
        if (gameProgress.coins_left > 0 && !betPlaced) {
            betPlaced = 1;
            lightUpAllLeds();
            playMoneyFallingSound();
        } else if (betPlaced && betAmount <= gameProgress.coins_left) {
            lightDownAllLeds();
            gameProgress.coins_left -= betAmount;
            printf("Now the slot machine will activate\n");
            remainingCoins(gameProgress.coins_left);
            break;
        }
    }
    }
    
    playSlotMachine(numSlots, betAmount);
}

int main() {
    //Initialize everything
    initUSART();
    initDisplay();
    initButton();
    initTimer();
    initADC();

    enableAllLeds();
    lightDownAllLeds();

    //Enable pin change interrupt for PCINT8
    PCICR |= _BV(PCIE1);
    PCMSK1 |= _BV(PCINT8);

    printf("\n\n============== One-armed bandit ==============");
    printf("\n\nPress button 2 to proceed!\n");
    
    char gameName[18] = "One-armed bandit ";

    while (status == 0) { //Scrolling text + start of the game
        scrollText(gameName, 500);
    }
    _delay_ms(BUTTON_DELAY);  
    
    //The game will continue as long as there are coins or the max is reached
    for (int i = 1; gameProgress.coins_left > 0 && gameProgress.coins_left < 9999; i++){
    
        //Setting the seed
        int seed;
        printf("\nTurn the potentiometer to seed rand()!\n");
        _delay_ms(2000);
        seed = getseed();
        srand(seed);
        printf("\nSeed: %d", seed);

        //Start of the game
        startGame();        
        
        //Print game progress
        printf("%d: %d coins available - %d coins bet - %d - %d coins won\n", 
        i, gameProgress.coins_left, gameProgress.betAmount
        , gameProgress.finalNumber, gameProgress.winAmount);
        
        //Show amount of coins left
        writeNumberAndWait(gameProgress.coins_left, 3000);
        
        //Show text to go again
        if(gameProgress.coins_left > 0){
            scrollText("GO AGAIN", 200);
        }
    }

    //Play sound for game over
    playEndSound();

    //END text of the game
    if(gameProgress.coins_left <= 0){ //If no coins left
        printf("You have no more coins left. Game over!\n");
        scrollText("You losed all your coins", 500);
    } else if (gameProgress.coins_left >= 9999){ //If max coins reached
        printf("You have won the game! Congratulations!\n");
        scrollText("Bank break", 500);
    }

    //Show total game time and total wins
    printf("You played for a total of %d minutes and %d seconds. During this time you won %d times.\n"
    ,  secondsCounter/60, secondsCounter%60, gameProgress.totalWins);
    
    return 0;
}