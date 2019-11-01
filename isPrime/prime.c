int isPrime(int n) {
  int f, result = 0;
  if(n>=2) {
    result = 1;
    for (f=2; f*f <= n; f++) {
      if((n%f) == 0) {
	result = 0;
	break;
      }
    }
  }
  return result;
}
