#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <err.h>

#define FALSE 0
#define TRUE  (!FALSE)

#define RATE 50000

// Iterator implementation
typedef struct _node *Node;
typedef struct _node {
    uint32_t value;
    Node    next;
} node_t;

typedef struct _iterator {
    Node start;
    Node curr;
    Node end;
} iterator_t;
typedef iterator_t *Iterator;

// Iterator interface
Node newNode (uint32_t value);
Iterator newIterator (void);
void destroyIterator (Iterator iterator);
void resetIterator (Iterator iterator);
int isEndIterator (Iterator iterator);
uint32_t nextIterator (Iterator iterator);
void pushIterator (Iterator iterator, uint32_t value);

// Count primes using an iterator for found primes
void countPrimes(Iterator iter, uint32_t max);

// Calculate the ceiling of the square root of an integer
uint32_t intSqrt (uint64_t num);

// Calculate progress as a double
double percentProgress (uint32_t step, uint32_t total);

int main (int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s max_prime\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int32_t max = atoi(argv[1]);

    if (max <= 0) {
        errx(EXIT_FAILURE, "max_prime must be a positive integer");
    }
    
    Iterator iter = newIterator();

    if (iter != NULL) {
        countPrimes(iter, (uint32_t)max);
    } else {
        errx(EXIT_FAILURE, "Failed to create iterator");
    }

    destroyIterator(iter);

    return EXIT_SUCCESS;
}

Node newNode (uint32_t value) {
    Node node = calloc(1, sizeof(node_t));

    if (node != NULL) {
        node->value = value;
        node->next = NULL;
    }

    return node;
}

Iterator newIterator (void) {
    Iterator iterator = calloc(1, sizeof(iterator_t));

    if (iterator != NULL) {
        iterator->start = NULL;
        iterator->curr = NULL;
        iterator->end = NULL;
    }

    return iterator;
}

void destroyIterator (Iterator iterator) {
    if (iterator != NULL) {
        // Free nodes
        Node curr, next;
        curr = iterator->start;
        while (curr != NULL) {
            next = curr->next;
            free(curr);
            curr = next;
        }

        // Free iterator
        free(iterator);
    }
}

void resetIterator (Iterator iterator) {
    if (iterator != NULL) {
        iterator->curr = iterator->start;
    }
}

int isEndIterator (Iterator iterator) {
    if (iterator == NULL) {
        errx(EXIT_FAILURE, "Cannot isEnd on null iterator");
    }

    return (iterator->curr == NULL);
}

uint32_t nextIterator (Iterator iterator) {
    if (iterator == NULL) {
        errx(EXIT_FAILURE, "Cannot next on null iterator");
    }
    if (iterator->curr == NULL) {
        errx(EXIT_FAILURE, "Cannot next at end of iterator");
    }

    uint32_t currVal = iterator->curr->value;
    iterator->curr = iterator->curr->next;
    return currVal;
}

void pushIterator (Iterator iterator, uint32_t value) {
    if (iterator != NULL) {
        Node new = newNode(value);
        if (new != NULL) {
            if (iterator->start == NULL) {
                iterator->start = new;
                iterator->curr = new;
            } else {
                iterator->end->next = new;
            }
            iterator->end = new;
        }
    }
}

double percentProgress (uint32_t step, uint32_t total) {
    return 100.0 * ((double) step / (double) total);
}

void countPrimes(Iterator iter, uint32_t max) {
    uint32_t primes = 0;
    uint32_t i;

    for (i = 2; i <= max; i++) {
        int prime = TRUE;
        uint32_t j = 0;
        resetIterator(iter);

        while (prime && !isEndIterator(iter) && j * j <= i) {
            j = nextIterator(iter);
            if (!(i % j)) {
                prime = FALSE;
            }
        }

        if (prime) {
            pushIterator(iter, i);
            primes++;
            if (primes < 100) {
                printf("%3d, ", i);
                if (!(primes % 5)) {
                    printf("\n");
                }
            } else if (primes == 100) {
                printf("%d\n", i);
            }

            if (!(primes % RATE)) {
                printf("\033[GFound %12d at %7.3lf%%...", 
                    primes, percentProgress(i, max)
                );
                fflush(stdout);
            }
        }
    }

    if (primes < 100 && primes % 5) {
        printf("\n");
    }

    printf("\033[G\033[KFound %12d primes.\n", primes);
}
