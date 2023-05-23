#define MAX_STRING 7
#define NAME_LENGTH 10
#include <stdio.h>
#include <string.h>
#include <usart.h>

void printFirstLetters(char string[MAX_STRING][NAME_LENGTH])
{
    printf("First letters: ");
    for (int i = 0; i < MAX_STRING; i++) {
        printf("%c", string[i][0]);
    }
}

void printLastLetters(char string[MAX_STRING][NAME_LENGTH]) 
{
  printf("Last letters: ");
  for (int i = 0; i < MAX_STRING; i++) {
      printf("%c", string[i][strlen(string[i]) - 1]);
    }
}

char *searchShortestName(char string[MAX_STRING][NAME_LENGTH]) 
{

  int shortest = strlen(string[0]); 
  int index = 0;

  for (int i = 1; i < MAX_STRING; i++) {
    int current = strlen(string[i]);
    if (current < shortest) {
      shortest = current;
      index = i;
    }
  }

  return string[index];
}

int main(void){
  initUSART(); 
  
  char names[MAX_STRING][NAME_LENGTH] = { "Lucky", "Wout", "Ard", "Martin", "Sean", "Anton", "Tjalma"};
    printFirstLetters(names);
    printf("\n");
    printLastLetters(names);
    
    char *smallest = searchShortestName(names);
    printf("\nShortest = %s\n", smallest);

    return 0;
}