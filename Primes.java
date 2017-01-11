import java.util.*;

class Primes {
    static final int RATE = 50000;

    public static void main (String[] argv) {
        if (argv.length != 1) {
            System.out.println("Usage: java Primes max_prime");
            System.exit(1);
        }

        int max = Integer.parseInt(argv[0]);

        if (max <= 0) {
            System.out.println("max_prime must be a positive integer");
            System.exit(1);
        }

        countPrimes(max);
    }

    private static double percentProgress(int step, int total) {
        return 100.0 * ((double) step / (double) total);
    }

    private static void countPrimes (int max) {
        Vector<Integer> primes = new Vector<Integer>();
        int nPrimes = 0;

        for (int i = 2; i <= max; i++) {
            Boolean prime = true;
            Iterator<Integer> iter = primes.iterator();
            int j = 0;

            while (iter.hasNext() && prime && j * j <= i) {
                j = iter.next();
                if (i % j == 0) {
                    prime = false;
                }
            }

            if (prime) {
                primes.add(i);
                nPrimes++;

                if (nPrimes < 100) {
                    System.out.format("%3d, ", i);
                    if (nPrimes % 5 == 0) {
                        System.out.println();
                    }
                } else if (nPrimes == 100) {
                    System.out.format("%3d%n", i);
                }

                if (nPrimes % RATE == 0) {
                    System.out.format("\u001B[GFound %12d primes at %7.3f%%...",
                        nPrimes, percentProgress(i, max)
                    );
                    System.out.flush();
                }
            }
        }

        if (nPrimes < 100 && nPrimes % 5 != 0) {
            System.out.println();
        }

        System.out.format("\u001B[G\u001B[KFound %12d primes.%n", nPrimes);
    }
}
