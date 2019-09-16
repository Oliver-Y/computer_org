#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

void find_digit(int number, int power,int base,char* result){
    if(number == 0){
      return;
    }
    if (pow(base,power) > number){
      //Add a 0
      *result = '0';
      result++;
      find_digit(number,power-1,base,result);
    }
    if (pow(base,power) <= number){
      //Add a 1
      *result = '1';
      result++;
      number -= pow(base,power);
      find_digit(number,power-1,base,result);
    }
}
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
  char *string;
  string = (char *) calloc(sizeof(char),100);
  //printf("intial string: %s",string);
  for (int i = 0; i < number_counter; i++){
    char result[100];
    int power = 0;
    int digit;
    while (pow(base,power) <= number[i]) power++;
    find_digit(number[i],power,base,string);
    printf("%s\n",string);
  }
  free(string);
}
