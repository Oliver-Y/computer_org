#include <stdio.h>
#include <stdlib.h>
int main(int argc, char *argv[]){
  char *string;
  string = calloc(sizeof(char),100);
  int a = 0;
  int offset = atoi(argv[1]);
  while((a = fgetc(stdin)) != EOF && a != 10){
    if(a == 32){
      *string = ' ';
    }
    else{
    *string = (a+offset < 122) ? (a+offset):(97+((a+offset)%122));
  }
  printf("%s",string);
  string++;
  }
  printf("\n");
}
