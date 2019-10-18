#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(){
  int comp_score = 0;
  int player_score = 0;
  uint32_t key = 0;
  uint8_t prediction = 0;
  uint16_t pattern[8];
  uint8_t last_guess;
  uint8_t same;
  uint8_t last_result;
  uint8_t current_result;
  int a = 0;
  srand(time(0));
  int random;
  for (int i = 0; i < 8; ++i){
    pattern[i] = 2;
  }
  while ((a=fgetc(stdin)) != EOF){
    if (a != 10) {
      //go random if no randoms.
      printf("Enter a guess between 0 and 1\n");
      pattern[key] = (a-48);
      random = rand() % 2;
      //configure key
      //key should be last_result, last_guess, current result.
      //See if its a win and compare results.
      if ((a-48) == prediction){
          //this is a win
          current_result = 1;
          comp_score ++;
      //    key = ((key << 1) | 1);
        }
      else{
        //this is a loss
        current_result = 0;
//        key = key << 1;
        player_score++;
      }
      if (last_guess == a){
        same = 1;
    //    key = ((key << 1) | 1);
      }
      else{
        same = 0;
//        key = key << 1;
      }
      //configure the key.
      key = ((key<<3) | (last_result << 2 | same << 1 | current_result));
      key &= 7;
      
      //If the pattern flag is true store a pattern
      //Compare to last guess update the key
      // evaluate predictions
      if (pattern[key] == 1 || pattern[key] == 0){
        prediction = pattern[key];
      }
      else{
        prediction = random;
      }
      //debuggin stuff
        /*printf("Computer choice: \t %d\n",prediction);
        printf ("Player choice \t %d\n",a - 48);
        printf ("last result \t %d\n",last_result);
        printf ("current result \t %d\n",current_result);
        printf ("same? \t %d\n",same);
        printf("key: \t %d\n",key);
        for (int i = 0; i < 8; ++i){
          printf("pattern %d: \t %d \n",i,pattern[i]);
        } */
        last_guess = a;
        last_result = current_result;


        printf("Computer Score: \t %d\n",comp_score);
        printf("Player Score: \t %d\n",player_score);
        //1. go random until a pattern has been found
        //2. When a pattern is found
          // Store 8 Permutations for predictions
          // 1. wins plays the same wins ?
          // 2. wins plays diff wins ?
          // 3. wins plays the same loses ?
          // 4. wins plays diff loses ?
          // 5. loses plays same wins ?
          // 6. loses plays diff wins ?
          // 7. loses plays same loses ?
          // 8. loses plays diff loses ?

        //Set up prediction

        //Compare prediction + increment scores.

        //Update predictions 2 things will update the prediction


    }

  }
}
