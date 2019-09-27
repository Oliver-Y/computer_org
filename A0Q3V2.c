#include <stdio.h>
#include <stdlib.h>
#include<time.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

int main(int argc,char *argv[]) {
  srand(time(0)+getpid());
  double random;
  char input[101];
  char output[101];
  double count = 1.0;
  FILE *fFile;

  if (argv[1] == NULL) {
    while (fgets(input, 101, stdin) != NULL) {
      random = (double) rand() / (RAND_MAX);
      if (random <= 1.0/count) {
        strcpy(output,input);
      }
      count++;
    }
  }
  else {
    fFile = fopen(argv[1],"r");
    if (fFile == NULL) printf("File doesn't exist\n");
    while (fgets(input, 101, fFile) != NULL) {
      random = (double) rand() / (RAND_MAX);
      if (random <= 1.0/count) {
        strcpy(output,input);
      }
      count++;
    }
  }

  printf("The quote is: %s", output);
  return 0;
}
