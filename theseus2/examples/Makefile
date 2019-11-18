prime:	prime.c isPrime.s
	gcc -g -o prime prime.c isPrime.s

prog:	prog.c pattern.s gcd.s
	gcc -g -o prog prog.c pattern.s gcd.s

cprog:	prog.c pattern.s gcd.s
	gcc -g -o cprog prog.c pattern.c gcd.c

clean:
	@rm -f *~ prog cprog *.o
	@echo All clean.

love:
	@echo Not war.
