use std::io::{stdout, Write};
use std::str::FromStr;
use std::{env, process};

const RATE: u32 = 50000;

fn main() {
    let mut args = env::args();

    if args.len() != 2 {
        println!("Usage: {} max_prime", args.nth(0).unwrap());
        process::exit(1);
    }

    let max = u32::from_str(args.nth(1).unwrap().as_str()).unwrap_or(0);

    if max <= 0 {
        println!("max_prime must bu a positive integer");
        process::exit(1);
    }

    count_primes(max);
}

fn percent_progress(step: u32, total: u32) -> f64 {
    100.0f64 * (step as f64 / total as f64)
}

fn count_primes(max: u32) {
    let mut n_primes = 0u32;
    let mut primes: Vec<u32> = Vec::new();
    let mut sout = stdout();

    for i in 2..(max + 1) {
        let mut prime = true;

        for j in &primes {
            if (j * j) > i {
                break;
            }

            if (i % j) == 0 {
                prime = false;
                break;
            }
        }

        if prime {
            primes.push(i);
            n_primes += 1;

            if n_primes < 100 {
                print!("{:3}, ", i);
                if (n_primes % 5) == 0 {
                    println!("");
                }
            } else {
                println!("{:3}", i);
            }

            if (n_primes % RATE) == 0 {
                print!(
                    "\x1B[GFound {:12} at {:7.3}%...",
                    n_primes,
                    percent_progress(i, max)
                );
                sout.flush().unwrap();
            }
        }
    }

    if n_primes < 100 && (n_primes % 5) != 0 {
        println!("");
    }

    println!("\x1B[G\x1B[KFound {:12} primes.", n_primes);
}
