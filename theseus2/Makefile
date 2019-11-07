theseus:	theseus.c theseus.h chase.s
	gcc -Wall -g -o theseus theseus.c chase.s -lncurses -lm

ctheseus:	theseus.c theseus.h cchase.c
	gcc -Wall -g -o ctheseus theseus.c cchase.c -lncurses -lm

clean:	
	@rm -rf *.o *~ *.dSYM
	@echo Made clean.

realclean:	clean
	@rm -f theseus ctheseus
	@echo Really.

love:
	@echo Not war.