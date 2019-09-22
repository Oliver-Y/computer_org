#include <stdio.h>
#include <stdlib.h>
int main(){
  //Tabs will always bring the character count to the next multiple of 8
  //Take in a String
  //Figure out the total character count
  //Figure out if there are tabs
  //Reallocate a string so that the tabs fill the character count up to multiples of 8.
  char *string;
  char *newstring;
  int counter = 0;
  int *spaces_needed;
  int offset = 0;
  int s_flag = 1;
  spaces_needed = calloc(5, sizeof(int));
  string = malloc(100 * sizeof(char));
  newstring = malloc(100*sizeof(char));
  fgets(string,50,stdin);
  while (*string != 10){
    if(*string == 9){
      for (int i = 0; i < 8-(counter%8); i++){
        *newstring = ' ';
        newstring++;
        offset++;
      }
      //spaces_needed++;
    }
      *newstring = *string;
      newstring++;
      string++;
      counter++;
  }
  newstring -= counter + offset;
  for (int i = 0; i < counter + offset; ++i){
    if(*newstring != 9){
      printf("%c",*newstring);
    }
    newstring++;
  }
//  printf("%s\n",newstring);
//  spaces_needed -= 2;
/*  for (int i = 0; i < 4; ++i){
    printf("%d\n",*spaces_needed);
    spaces_needed++;
  } */
  return 1;
}
