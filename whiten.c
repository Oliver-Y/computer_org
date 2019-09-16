#include <stdio.h>
int main(){
  char a;
  char b;
  while ((a = fgetc(stdin)) != EOF) {
    while (a == 32) a = fgetc(stdin);
    b = fgetc(stdin);
    while (b == 32) b = fgetc(stdin);
    if (a != b && b!= 10){
      printf("%c\n",a);
      }
    }
  }
