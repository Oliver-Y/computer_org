#include <stdio.h>
#include <stdlib.h>
#include<time.h>
#include <unistd.h>


int main() {
  srand(time(0)+getpid());

  //initialize variables
  int random;
  int prediction;
  int currentInput;
  int userScore = 0;
  int compScore = 0;
  unsigned short previousPreviousResult = (short) (rand() % 2);
  unsigned short previousInput = (short) (rand() % 2);
  unsigned short previousResult = (short) (rand() % 2);
  unsigned short key = 0x0000;

  //initialize guess Array with random integers between 0 and 1
  int *guessArray = malloc(8*sizeof(int));
  for(int i=0; i<8;i++) {
    guessArray[i] = (int) (rand() % 2);
  }
  // printf("\n%d\n", guessArray[key]);

  while ((currentInput=fgetc(stdin)) != EOF) {
    //convert ascii to binary;
    currentInput -=48;
    //proceed only if currentInput is 0 or 1
    if (currentInput == 0 || currentInput == 1) {

      //make a prediction
      prediction = guessArray[key];
      printf("%d\n", prediction);

      //shift previousResult to previousPreviousResult
      previousPreviousResult = previousResult;

      //check if guess is correct & updating scores & current result
      if (currentInput == prediction) {
        compScore ++;
        previousResult = 1;
      }
      else {
        userScore ++;
        previousResult = 0;
      }
      printf("Computer score: %d\n", compScore);
      printf("User score: %d\n", userScore);
      if (userScore == 30) {
      	printf("User wins!");
      	break;
      }
      if (compScore == 30) {
      	printf("Computer wins!");
      	break;
      }

      //update guessArray
      guessArray[key] = currentInput;

      //shift currentInput to previousInput
      previousInput = currentInput;

      //update key
      //order: previousPreviousResult;previousInput;previousResult  : currentInput
      key = 0x0000;
      key = (previousPreviousResult<<2|previousInput <<1|previousResult);
    }
  }
  return 0;
}
