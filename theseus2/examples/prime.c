// Filter and echo primes on input.
// (c) 2015-2019 duane a. bailey
#include <stdio.h>

extern int isPrime(int n);

int main(int argc, char **argv)
{
  int n;

  while (1 == scanf("%d",&n)) {
    if (isPrime(n)) printf("%d\n",n);
  }
}
