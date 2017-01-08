#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <err.h>

#define FALSE 0
#define TRUE  (!FALSE)

#define MAX_PRIMES (1<<30)

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

uint32_t intSqrt (uint64_t num) {
    uint64_t root = (uint32_t) ~0;
    uint32_t mask;

    for (mask = 1 << 31; mask > 0; mask >>= 1) {
        root &= ~mask;
        if (root * root < num) {
            root |= mask;
        }
    }

    return root;
}

void countPrimes(Iterator iter) {
    uint32_t primes = 0;
    uint32_t i;

    for (i = 2; i < MAX_PRIMES; i++) {
        int prime = TRUE;
        uint32_t j = 0;
        uint32_t root = intSqrt(i);
        resetIterator(iter);

        while (prime && !isEndIterator(iter) && j <= root) {
            j = nextIterator(iter);
            if (!(i % j)) {
                prime = FALSE;
            }
        }

        if (prime) {
            pushIterator(iter, i);
            primes++;
            if (primes < 100) {
                printf("%d, ", i);
            } else if (primes == 100) {
                printf("\n");
            }

            if (!(primes % 50000)) {
                printf("\e[GFound %12d at %7.3lf%%...", 
                    primes, percentProgress(i, MAX_PRIMES)
                );
                fflush(stdout);
            }
        }
    }

    printf("\e[G\e[KFound %12d primes.\n", primes);
}

int main (void) {
    Iterator iter = newIterator();

    if (iter != NULL) {
        countPrimes(iter);
    } else {
        errx(EXIT_FAILURE, "Failed to create iterator");
    }

    destroyIterator(iter);

    return EXIT_SUCCESS;
}
