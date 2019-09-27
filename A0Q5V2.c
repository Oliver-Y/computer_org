#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

void mex(int n, int v[]) {
  int mex = 0;
  int *mexArray = malloc((n+1)*sizeof(int));

  for (int x=0;x<(n+1);x++){
    mexArray[x]=-1;
    }

  for (int x=0;x<n;x++) {
    if (v[x] >=n) continue;
    else mexArray[v[x]]=v[x];
  }
  for (int x=0;x<(n+1);x++) {
    if (mexArray[x] == -1) {
      mex = x;
      break;
    }
  }
  printf("The minimum excluded integer is %d\n", mex);
}

int main(int argc, char *argv[]) {
  char line[101];
  int used = 0;
  int capacity = 20;
  int *numArray = malloc(capacity* sizeof(int));
  if (argv[1] == NULL) {
    while (fgets(line, 101, stdin) != NULL){
      numArray[used] = atoi(line);
      used++;

      if (used == capacity) {
        capacity *= 2;
        numArray = realloc(numArray, capacity*sizeof(int));
      }
    }
  }
  else {
    FILE *fFile;
    fFile = fopen(argv[1],"r");
    if (fFile == NULL) printf("File doesn't exist\n");
    while (fgets(line, 101, fFile) != NULL){
      numArray[used] = atoi(line);
      used++;

      if (used == capacity) {
        capacity *= 2;
        numArray = realloc(numArray, capacity*sizeof(int));
      }
  }
}

  // for(int x = 0; x < used; x++) {
  //   printf("%d ",numArray[x]);
  // }
  // printf("\n");

  mex(used,numArray);
  free(numArray);
}
