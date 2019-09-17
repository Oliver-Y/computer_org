#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

int main(int argc, char *argv[]){
  int base = atoi(argv[1]);
  char a = 0;
  int number[100];
  int number_counter = 0;
  char temp_buffer[100] = "";
  int buffer_counter = 0;
  while((a = fgetc(stdin)) != EOF){
    temp_buffer[buffer_counter++] = a;
    if (a == 32 || a == 10){
      buffer_counter = 0;
      number[number_counter++] = atoi(temp_buffer);
    }
  }
  for (int i = 0; i < number_counter; i++){
    char result[100];
    char digitstr;
    int power = 0;
    int digit;
    int num = number[i];
    while (pow(base,power) <= num) power++;
    for (int j = power-1; j >= 0; --j){
//      printf("num is: %d\n", num);
      digit = num / pow(base, j);
//      printf("digit is: %d\n", digit);
      num -=  pow(base, j)*digit;

       if (digit < 10) {
         digitstr=(char)(digit+48);
       }
       else if (10 <= digit && digit < 36){
         digitstr = (char)(digit+55);
       }
       else {
         printf("Base too big");
       }
       strcat(result, &digitstr);
    }

    printf("%s\n", result);

    //if (number[i] % pow(base,i) != number[i])
  }



  /*for (int i = 0; i < 100; i++ ) {
    int digit;
    if (number[i] == -1) break;
    if (number[i] == 0) {
      printf("0\n");
      continue;
    }
    char* result = "";
    char digitstr;

    for (int x=0; pow(base,x) <= number[i]; x++) {
      char intermediaters[100] = "";

      int digit = (((number[i] % (int)pow(base, x+1)-(number[i] % (int)pow(base, x)))
            /pow(base, x)));
          digitstr = (char) (digit + 48);
          if (digit < 10) {
            digitstr=(char)(digit+48);
          }
          else if (10 <= digit && digit < 36){
            digitstr = (char)(digit+55);
          }
          else {
            printf("Base too big");
          }

      strcat(intermediaters, &digitstr);
      strcat(intermediaters, result);
      result = intermediaters;

    }

    printf("%s\n", result);
  } */

}
