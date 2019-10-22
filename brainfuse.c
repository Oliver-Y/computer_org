#include <stdio.h>
#include <stdlib.h>
//Set up the memory
//Brainfuse
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
  //Useful flags
  int dis_flag = 0;
  int loop_flag = 0;
  int skips = 0;
  int b_flag = 0;

  //Load argument into instruction poniter
  int arg_length = 0;
  while (*argv[1] != NULL){
    *ip = *argv[1];
    ip++;
    argv[1]++;
    arg_length++;
  }
  ip -= arg_length;

//Loop
while((*ip != 0 || loop_flag == 1) && !b_flag)
  {
    while(loop_flag){
      ip--;
      if (*ip == ']'){
        skips++;
      }
      if(*ip == '[') {
          skips--;
          if(skips == 0){
            loop_flag = 0;
          }
      }

    }
    if(!dis_flag || *ip == ']'){
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
          //Trigger some flag.
        case '[':
        if(*dp == 0){
          dis_flag = 1;
        }
          break;
        case ']':
          dis_flag = 0;
          if(*dp != 0 && dis_flag != 1){

            loop_flag = 1;
          }
          if(*dp == 0){
          }
          break;
        case ',':
          *dp = fgetc(stdin);
          if (*dp == -1){
            b_flag = 1;
          }
          break;
        case '.':
          if (*dp <= 31 && *dp != 10){
            printf("%d",*dp);
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
    }
    ip++;
  }
  return 1;
}
