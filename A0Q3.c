#include <stdio.h>
#include <stdlib.h>
#include<time.h>

int main(int argc,char *argv[]) {
  srand(time(0));
  FILE *fFile;
  int count = 0;
  char line[101];
  int r = 0;
  r = rand();
  int count2 = 0;

  if (argv[1] == NULL) {
    fFile = tmpfile();
    while (fgets(line, 101, stdin) != NULL) {
      fputs(line, fFile);
      count++;
    }
  } else {
    fFile = fopen(argv[1],"r");
    if (fFile == NULL) printf("File doesn't exist\n");
    while (fgets(line, 101, fFile) != NULL) {
      count++;
    }
  }

  printf("count = %d\n", count);
  printf("r = %d\n\n",r);
  rewind(fFile);

  while (fgets(line, 101, fFile) != NULL) {
    if ((r % count) == count2) {
      break;
    }
    count2++;
  }
  
  printf("The quote is: %s", line);
  return 0;
}
