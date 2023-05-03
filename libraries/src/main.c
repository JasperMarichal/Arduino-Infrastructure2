#include <LEDlib.h>
#include <util/delay.h>

int main(void)
{
    enableAllLeds(); // Enable all four LEDs
    lightDownAllLeds(); // Turn off all four LEDs
    
    while (1) // Loop forever
    {
         lightUpAllLeds(); // Turn on all four LEDs
         _delay_ms(1000); // Delay for one second
        
         lightDownAllLeds(); // Turn off all four LEDs
         _delay_ms(1000); // Delay for one second
        
        // lightToggleLed(2); // Toggle LED #2
        // _delay_ms(500); // Delay for half a second
        
        // lightToggleLed(0); // Toggle LED #0
        // _delay_ms(500); // Delay for half a second

        // lightDownAllLeds(); // Turn off all four LEDs
        // _delay_ms(1000); // Delay for one second
        // lightUpAllLeds(); // Turn on all four LEDs
        // _delay_ms(1000); // Delay for one second 

      //  lightUpOneLed(2);
      //  _delay_ms(1000);

      //  lightDownOneLed(2);
      //  _delay_ms(1000);

        //   dimLed(2, 10, 20);
        //   fadeLed(3,10);
    }
}