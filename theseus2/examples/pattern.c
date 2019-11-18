// A program to print a table of relatively prime (+) values.
// The number of +'s in a row is Euler's totient function; more than one
// * in a row indicates that row number (numbered starting at 1) is prime.
#include <stdio.h>
/* pattern(stdout,10) draws:
 +
 + *
 + + *
 + * + *
 + + + + *
 + * * * + *
 + + + + + + *
 + * + * + * + *
 + + * + + * + + *
 + * + * * * + * + *
*/
extern int gcd(int,int);
void pattern(FILE *f, int range)
{
  int row, col, rp;
  for (row = 1; row <= range; row++) { // note non-idiomatic use, for math
    for (col = 1; col <= row; col++) { // same here
      rp = gcd(row,col);
      fputc(' ',f);
      if (rp == 1) fputc('+',f);
      else fputc('*',f);
    }
    fputc('\n',f);
  }
}
