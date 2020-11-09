
#ifndef INTCODE__H_
#define INTCODE__H_

/* Defines the size of one memory cell / register in the intcode machine. */
#define INTC_WORD long

typedef struct {
    INTC_WORD *ram;
    long ramsize;
    /* instruction pointer */
    INTC_WORD ip;
} intcode;


void intc_dtor(intcode *vm);
int intc_init(const char *memfile, intcode *result);
int intc_copy(const intcode * const original, intcode * clone);
int intc_run(intcode * const vm);
#endif
