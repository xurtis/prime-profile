CC=clang
CFLAGS=-O3

EVERYTHING=c-primes

all:	$(EVERYTHING)

clean:
	rm -rf $(EVERYTHING)

c-primes:	primes.c
	$(CC) $(CFLAGS) -o $@ $^
