#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int bigEndian() {
  short test = 0x0001;
  char* x = (char*) &test;
  //returns 0 if big endian, return 1 if little endian
  //big endian   : 00 01
  //little endian: 01 00
  //return value:   ^
  return *x;
}

int main(int argc, char *argv[]){
  if (strcmp (argv[0],"little") == 0) {
    exit(!bigEndian());
  }
  else {
    exit(bigEndian());
  }

}
