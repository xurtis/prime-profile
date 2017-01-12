CC=clang
CFLAGS=-O3
JC=javac
RC=rustc
RFLAGS=-C opt-level=3

EVERYTHING=c-primes Primes.class rs-primes

all:	$(EVERYTHING)

clean:
	rm -rf $(EVERYTHING) primes.pyc

c-primes:	primes.c
	$(CC) $(CFLAGS) -o $@ $^

rs-primes: 	primes.rs
	$(RC) $(RFLAGS) -o $@ $^

%.class:	%.java
	$(JC) $^
