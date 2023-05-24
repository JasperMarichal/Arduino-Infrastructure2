#include <util/delay.h>
#include <avr/io.h>
#include <usart.h>
#include <display.h>

int main() {

  initUSART();
  initDisplay();

  while(1) {
    for ( int i = 0; i < 4; i++ ) {
      writeCharToSegment(i, 'N');
      _delay_ms( 1000 );
      }
   
    writeString("TEST");
    _delay_ms( 1000 );

    for ( int i = 0; i < 10000; i ++ ) {
      writeString("HANS");
    }
    writeStringAndWait("HOOP", 10000);
  }

  return 0;
}