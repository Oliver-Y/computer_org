#include <stdio.h>
#include <stdlib.h>
//Set up the memory
//Brainfuse
int* jump_back(int *ip){
  int loop_flag = 1;
  int skips = 0;
  ip--;
  while(loop_flag){
    ip--;
    if (*ip == ']'){
      skips++;
    }
    if(*ip == '[') {
        if(skips <= 0){
          loop_flag = 0;
          return ip;
        }
        skips--;
    }
  }
}

int* jump_forward(int *ip){
//  ip--;
  int loop_flag = 1;
  int skips = 0;
  while(loop_flag){
    ip++;
    if(*ip == '['){
      skips++;
    }
    if(*ip == ']'){
      skips--;
      if(skips <= 0){
        loop_flag = 0;
        return ip;
      }
    }
  }
}

int main(int argc, char * argv[]){
  //Initializing BrainFuse Memory
  int memory_size = 1000;
  int* memory;
  memory = calloc(1000,sizeof(int));
  int *fuse;
  fuse = memory + memory_size/2;
  //Sets up 0 point as fuse.
  *fuse = 0;
  //Commas takes it into standard in
  int counter = 0;
  //Data pointer and instruction pointer
  int *dp = fuse + 1;
  int *ip = memory +1;
  //Keep track of starting location for loop
  int *lp;
  //Break flag when the program is suppose to end at the end.
  int b_flag = 0;
  //Load arguments into instruction pointer
  int arg_length = 0;
  while (*argv[1] != NULL){
    *ip = *argv[1];
    ip++;
    argv[1]++;
    arg_length++;
  }
  ip -= arg_length;
//Loops when the IP is not = 0 or when its suppose
//to be looping and the whole thing is not broken.
while(*ip != 0 && !b_flag)
  {
      switch(*ip){
        case '+':
          *dp += 1;
          break;
        case '-':
          counter++;
          *dp -= 1;
          break;
        case '>':
          dp++;
          break;
        case '<':
          dp--;
          break;
        case '[':
        if(*dp == 0){
          ip = jump_forward(ip);
        }
          break;
        case ']':
          if(*dp != 0){
            ip = jump_back(ip);
          }
          break;
        case ',':
          *dp = fgetc(stdin);
          if(*dp == EOF){
            *dp = 0;
          }
          break;
        case '.':
          if (*dp <= 31 && *dp != 10){
            printf("%d\n",*dp);
          }
          else{
            printf("%c",*dp);
          }
          break;
        case '0':
          b_flag = 1;
          break;
        default:
          break;
      }
    //  printf("this is the character %c:\n",*ip);
    ip++;
  }
  return 1;
}
