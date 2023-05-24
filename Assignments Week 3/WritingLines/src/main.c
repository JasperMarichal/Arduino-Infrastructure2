#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <usart.h>
#include <buttonlib.h>

void main(void) { //writing_lines
    initUSART();
    enableButton(0);
    int bytes = 0;

    while(true) {
        if (buttonPushed(0)) {
            for (int i = 1; i <= 100; i++) {
                printf("%d: I am not allowed to speak in class.\n", i);
                write_lines_on_heap("I am not allowed to speak in class.", &bytes);
            }
            break;
        }
    }
}

void write_lines_on_heap(char sentence[], int *bytes)
{
    int len = strlen(sentence);
    int size = (len + 1) * sizeof(char);
    char *s = malloc(size);

    if (s) {
        *bytes += size;
        strcpy(sentence, s);
        s[len] = '\0';
        printf("\"%s\" is on heap ...\n%d bytes are now occupied on heap ...\n\n", s, *bytes);
        free(s);
    } else {
        printf("\nMemory allocation fault...\n%d bytes are now occupied on heap ...\n\n", *bytes);
    }
}