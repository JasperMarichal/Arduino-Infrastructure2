#include <ledlib.h>

int main(void)
{
      enableAllLeds(); // Enable all four LEDs
      lightDownAllLeds(); // Turn off all four LEDs
    
      while (1) // Loop forever
      {
      dimLed(0, 20, 1000);            
      dimLed(1, 40, 1000);
      dimLed(2, 60, 1000);
      dimLed(3, 80, 1000);
      }
}