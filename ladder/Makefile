# What, on earth, do you expect to find here?
# (N.B. The use of code in search.s is governed by code in ladder.c)
ladder:	ladder.c ladder.h search.s
	gcc -Wall -g -o ladder ladder.c search.s

tests:
	@(cd test-suite; bash suite)

clean:
	@rm -rf *~ \#* *.dSYM

realclean:	clean
	@rm -f ladder

love:
	@echo Not war.
