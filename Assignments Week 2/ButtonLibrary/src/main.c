#include <buttonLib.h>
#include <usart.h>

void main(){

    initUSART();

    enableButton(0);
    enableButton(1);
    enableButton(2);

    while(1) {

        if (buttonPushed(0)) {
            printf("Button 0 pushed\n");
        }

        if (buttonPushed(1)) {
            printf("Button 1 pushed\n");
        }

        if (buttonPushed(2)) {
            printf("Button 2 bushed\n");
        }
    }
}