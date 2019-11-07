// C for computing GCD, recursively, using Euclid's algorithm.  What a dude.
extern int gcd(int,int);
int gcd(int a, int b)
{
    int result;
    if (a == 0) result = b;
    else if (a > b) result = gcd(b,a);
    else result = gcd(a,b-a);
    return result;
}
