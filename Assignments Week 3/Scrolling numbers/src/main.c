#include <util/delay.h>
#include <avr/io.h>
#include <usart.h>
#include <display.h>

int main()
{
    initUSART();
    initDisplay();

    int numbers[10] = {5, 6, 7, 8, 9, 0, 1, 2, 3, 4};

    int number = 1234;

    while (1)
    {
        for (int i = 0; i < 10; i++)
        {
            writeNumberAndWait(number, 1000); // Adjust the delay according to your desired speed
            number = (number % 1000) * 10 + numbers[i % 10];
        }
    }
    return 0;
}
