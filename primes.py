#!/usr/bin/env python2

from sys import stdout, argv, exit

RATE = 5000

def percentProgress(step, total):
    return 100.0 * (float(step) / float(total))

def countPrimes(maxVal):
    nPrimes = 0
    primes = []

    for i in xrange(2, maxVal):
        prime = True

        for p in primes:
            # Exit loop if not prime or not going to find factor
            if (p * p > i):
                break

            # Exit loop if proven not prime
            if ((i % p) == 0):
                prime = False
                break

        if prime:
            primes.append(i)
            nPrimes += 1
            if (nPrimes < 100):
                stdout.write("{:3}, ".format(i))
                if not nPrimes % 5:
                    stdout.write("\n")
            elif (nPrimes == 100):
                stdout.write("{:3}\n".format(i))

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

    if (maxVal <= 0):
        print "max_val must be a positive integer"
        exit(1)

    countPrimes(maxVal)
