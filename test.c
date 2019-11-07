#include <stdio.h>
#include <stdlib.h>

int main(){
  char * string;
  string = malloc(1000*sizeof(char));
  //string2 = malloc(1000*sizeof(char));
  fgets(string,500,stdin);
  do{
    string++;
    if(*string == 10){
      fgets(string,500,stdin);
      printf("%s",string);
    }
  }
  while(*string != 10);
  printf("%s",string);
  return 1;
}
