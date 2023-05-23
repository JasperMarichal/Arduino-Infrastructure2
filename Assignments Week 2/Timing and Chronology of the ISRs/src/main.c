#include <util/delay.h>
#include <avr/io.h>
#include <LEDlib.h>
#include <buttonLib.h>
#include <avr/interrupt.h>

ISR(PCINT1_vect){
 if (buttonPushed(0)) {
    lightUpMultipleLeds(0b0000110);
    _delay_ms(5000);
    lightDownMultipleLeds(0b0000110);
    _delay_ms(5000);
  }
  if (buttonPushed(1)) {
    for (int i = 0; i < 7; i++) {
    lightUpOneLed(1);
    _delay_ms(500);
    lightDownOneLed(1);
    _delay_ms(500);
    }
  }
}

void initButton(){
  enableButton(0);
  enableButton(1);
  PCICR |= (1 << PCIE1);
  PCMSK1 |= (1 << PCINT9) | (1 << PCINT10);
  sei();
}

int main() {
  //Leds
enableAllLeds();
lightDownAllLeds();
//Buttons
initButton();

while(1){
    lightUpMultipleLeds(0b00001100);
  _delay_ms(2000);
  lightDownMultipleLeds(0b00001100);
  _delay_ms(2000);
}
  return 0;
}

/*Questions:
Does LED D1 continue to flash when the ISR has started?
No
Does LED D1 and D2 continue to flash after the ISR has been handled (ie is it over)?
No
If LED D1 and D2 were on before starting the ISR, are they still on after running the ISR?
Yes
If LED D1 and D2 were off before starting the ISR, are they still off after running the ISR?
Yes
If LEDs D1 and D2 were on for only 0.5 seconds before starting the ISR, will they keep their status for 1.5 seconds after performing the ISR?
No
*/


/*EXPANSION*/
/*Questions:
Activate the functionality associated with button S2 after starting the program.
How does this affect the execution of the program?
This will not affect the execution of the program
Activate the functionality associated with button S2 during the execution of the ISR associated with pressing button S1. How does this affect the execution of the program / the ISR?
The ISR for button S2 will be handled immediately before resuming the execution of the ISR for button S1.
*/