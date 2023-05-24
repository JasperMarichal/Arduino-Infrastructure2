#include <util/delay.h>
#include <avr/io.h>
#include<avr/sfr_defs.h>
#include<usart.h>
#include <avr/interrupt.h>

void enableButton( int button );
void initButtons();
int buttonPushed( int button );
int buttonReleased( int button );
int readButton();