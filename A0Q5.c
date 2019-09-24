#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

void mex(int n, int v[]) {
  int minimum = 0;
  int x = 0;
  while(x<n) {
    if (v[x] == minimum) {
      minimum ++;
      x = -1;
    }
    x++;
  }
  printf("The minimum excluded integer is %d\n", minimum);
}

int main(int argc, char *argv[]) {
  char line[101];
  int used = 0;
  int capacity = 20;
  int *numArray = malloc(capacity* sizeof(int));
  while (fgets(line, 101, stdin) != NULL){
    numArray[used] = atoi(line);
    used++;

    if (used == capacity) {
      capacity *= 2;
      numArray = realloc(numArray, capacity*sizeof(int));
    }
  }
  for(int x = 0; x < used; x++) {
    printf("%d ",numArray[x]);
  }
  printf("\n");

  mex(used,numArray);
  free(numArray);
}
