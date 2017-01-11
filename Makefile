CC=clang
CFLAGS=-O3
JC=javac

EVERYTHING=c-primes Primes.class

all:	$(EVERYTHING)

clean:
	rm -rf $(EVERYTHING)

c-primes:	primes.c
	$(CC) $(CFLAGS) -o $@ $^

%.class:	%.java
	$(JC) $^
