#include <stdio.h>
#include <stdlib.h>
int main(){
  char *string;
  char *newstring;
  int counter = 0;
  int *spaces_needed;
  int offset = 0;
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
  return 1;
}
