#include <stdio.h>
#include <stdlib.h>
int main(){
  char *newstring;
  int counter = 0;
  int *spaces_needed;
  int offset = 0;
  spaces_needed = calloc(100, sizeof(int));
  newstring = calloc(500,sizeof(char));
  char a;
  while ((a = fgetc(stdin)) != EOF){
    if (a == 9){
      for (int i = 0; i < 8-(counter%8); i++){
        *newstring = ' ';
        newstring++;
        offset++;
      }
    }
    else{
      *newstring = a;
      newstring++;
      counter++;
    }
  }
  newstring -= counter+offset;
  printf("%s",newstring);
}
