#define MAX_INT 5
#define SEVENFOLD 7
#include <stdio.h>
#include <stdlib.h>
#include <usart.h>

void printArray(int array[MAX_INT]) {
    printf("Content of array:\n");
    for (int i = 0; i < MAX_INT; i++) {
        printf("Address: %p has value: %d\n", &array[i], array[i]);   
    }
}

void makeArray(int array[]) {
    for (int i = 0; i < MAX_INT; i++) {
        array[i] = i * SEVENFOLD;
    }
}

int main(void) {
    initUSART();

  int array[] = { 0, 0 ,0 ,0, 0 };
      printArray(array);
      makeArray(array);
      printArray(array);
      printf("\n");

      return 0;
}