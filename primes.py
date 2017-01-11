#!/usr/bin/env python2

from sys import stdout, argv, exit

RATE = 5000

def intSqrt(num):
    root = (1 << 32) - 1
    mask = 1 << 31

    while mask > 0:
        root &= ~mask
        if (root * root < num):
            root |= mask
        mask >>= 1

    return root

def percentProgress(step, total):
    return 100.0 * (float(step) / float(total))

def countPrimes(maxVal):
    nPrimes = 0
    primes = []

    for i in xrange(2, maxVal):
        prime = True
        root = intSqrt(i)

        for p in primes:
            # Exit loop if not prime or not going to find factor
            if (p > root):
                break

            # Exit loop if proven not prime
            if ((i % p) == 0):
                prime = False
                break

        if prime:
            primes.append(i)
            nPrimes += 1
            if (nPrimes < 100):
                stdout.write("{}, ".format(i))
                if not nPrimes % 5:
                    stdout.write("\n")
            elif (nPrimes == 100):
                stdout.write("{}\n".format(i))

            if ((nPrimes % RATE) == 0):
                stdout.write("\033[GFound {:12d} at {:7.3f}%...".format(
                    nPrimes, percentProgress(i, maxVal)
                ))
                stdout.flush()

    if nPrimes < 100 and primes % 5:
        stdout.write("\n")

    stdout.write("\033[G\033[KFound {:12d} primes.\n".format(nPrimes))

if __name__ == "__main__":
    if len(argv) != 2:
        print "Usage: {} max_prime".format(argv[0])
        exit(1)

    maxVal = int(argv[1])

    countPrimes(maxVal)
