#include <util/delay.h>
#include <avr/io.h>
#include <LEDlib.h>
#include <buttonLib.h>
#include <avr/interrupt.h>

#define LED_PORT PORTB
#define BUTTON_PORT PORTC
#define BUTTON_PIN PINC
#define LED_DDR DDRB // register for all LEDs
#define BUTTON_DDR DDRC
#define BUTTON1 PC1
#define LED1 PB2
#define LED2 PB3
#define LED3 PB4
#define LED4 PB5

/*Create a program that demonstrates the timing and chronology of ISRs (Interrupt Service Routines).
When starting this program, LEDs D1 and D2 start flashing together on and
off with an interval of 4 seconds (2 seconds together on, 2 seconds together off).*/

int main()
{
  LED_DDR |= _BV(LED1) | _BV(LED2) | _BV(LED3) | _BV(LED4);  // we'll use all leds
  LED_PORT |= _BV(LED1) | _BV(LED2) | _BV(LED3) | _BV(LED4); // turn all leds off
  BUTTON_DDR &= ~_BV(BUTTON1);                               // we'll use button1
  BUTTON_PORT |= _BV(BUTTON1);                               // enable pull-up
  PCICR |= _BV(PCIE1);                                     // enable interrupt for PCINT8..14
  PCMSK1 |= _BV(BUTTON1);                                 // enable interrupt for button1
  sei();                                               // enable interrupts

  if (bit_is_clear(BUTTON_PIN, BUTTON1))
  {
    _delay_us(5000);
    if (bit_is_clear(BUTTON_PIN, BUTTON1))
    {
      // Check if both LED3 and LED4 are off
      if (bit_is_set(LED_PORT, LED3) || bit_is_set(LED_PORT, LED4))
      {
        // turn off LED1 adn LED2, then turn on both LED3 and LED4
        LED_PORT |= _BV(LED1);
        LED_PORT |= _BV(LED2);
        // Turn on both LED3 and LED4
        LED_PORT &= _BV(LED3);
        LED_PORT &= _BV(LED4);

        // trying to make it light up if pushed, and light off when pushed
        if (buttonPushed(BUTTON1))
        {
          LED_PORT &= ~_BV(LED1);
          LED_PORT &= ~_BV(LED2);
        }
      }
      // add somewhere buttonPushed and buttonReleased
    }
  }
  while (1)
  {
    LED_PORT &= ~_BV(LED1); // turn led1 on
    LED_PORT &= ~_BV(LED2); // turn led2 on
    _delay_ms(2000);
    LED_PORT |= _BV(LED1); // turn led1 off
    LED_PORT |= _BV(LED2); // turn led2 off
    _delay_ms(2000);
  }
  return 0;

  /*if (bit_is_clear(BUTTON_PIN, BUTTON1))
  {
    _delay_us(5000);
    if (bit_is_clear(BUTTON_PIN, BUTTON1))
    {
      if (bit_is_set(LED_PORT, LED3)||bit_is_set(LED_PORT, LED4))
      {
        LED_PORT &= ~_BV(LED3);
        LED_PORT &= ~_BV(LED4);
      }
      else
      {
        LED_PORT |= _BV(LED3);
        LED_PORT |= _BV(LED4);
      }
    }
  }*/
}

/*Questions:
Does LED D1 continue to flash when the ISR has started?
Does LED D1 and D2 continue to flash after the ISR has been handled (ie is it over)?
If LED D1 and D2 were on before starting the ISR, are they still on after running the ISR?
If LED D1 and D2 were off before starting the ISR, are they still off after running the ISR?
If LEDs D1 and D2 were on for only 0.5 seconds before starting the ISR, will they keep their status for 1.5 seconds after performing the ISR?*/