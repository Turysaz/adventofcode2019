#include "intcode.h"
#include "../common.h"
#include "../read.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define ICIMG_SEP " \n,"

enum intcerr {
    IC_IMGIO_ERR = -1,
    IC_BAD_IMAGE = -2,
    IC_BAD_OPCODE = -100,
    IC_SEGFAULT = -101,
    IC_BADIP = -102
};

enum opcodes { OPC_ADD = 1, OPC_MUL = 2, OPC_STP = 99 };

/* get instruction count */
int icnt(char *program) {
    int i = -1;
    int count = 1;

    while (program[++i] != '\0') {
        count += program[i] == ',' && program[i + 1] != '\0';
    }

    return count;
}

void intc_dtor(intcode *vm) {
    if (vm->ram) {
        free(vm->ram);
        vm->ram = NULL;
        vm->ramsize = 0;
        vm->ip = 0;
    }
}

int intc_copy(const intcode *const original, intcode *clone)
{
    int retcode = 0;
    INTC_WORD *newram;
    long count = original->ramsize;

    if(!(newram = malloc(count * sizeof(INTC_WORD))))
    {
        retcode = MALLOC_ERR;
        goto clean;
    }

    memcpy(newram, original->ram, count * sizeof(INTC_WORD));

    clone->ramsize = count;
    clone->ip = original->ip;
    clone->ram = newram;

clean:
    return retcode;
}

int intc_init(const char *memfile, intcode *result) {
    char *imgbuf;
    long imgsize;
    int icount;
    char *lasttok;
    char *token;
    INTC_WORD *ram;
    int i = 0;
    int retcode = 0;

    imgsize = frdall(memfile, &imgbuf);
    if (imgsize < 0) {
        retcode = IC_IMGIO_ERR;
        goto clean;
    }

    icount = icnt(imgbuf);
    ram = malloc(icount * sizeof(INTC_WORD));
    if (!ram) {
        retcode = MALLOC_ERR;
        goto clean;
    }

    lasttok = imgbuf;
    token = strtok(imgbuf, ICIMG_SEP);
    while (token) {
        if (token - 5 > lasttok) {
            retcode = IC_BAD_IMAGE;
            goto clean;
        }

        ram[i++] = atol(token);

        lasttok = token;
        token = strtok(NULL, ICIMG_SEP);
    }

    if (i != icount) {
        retcode = IC_BAD_IMAGE;
        goto clean;
    }

    result->ip = 0;
    result->ram = ram;
    result->ramsize = icount;
    retcode = 0;

clean:
    if (imgbuf) {
        free(imgbuf);
    }

    if (retcode < 0 && ram) {
        free(ram);
    }

    return retcode;
}

/*get RAM*/
int intc_gram(const intcode *const vm, INTC_WORD address, INTC_WORD *result) {
    if (address < 0 || address > vm->ramsize) {
        return IC_SEGFAULT;
    }

    *result = vm->ram[address];
    return 0;
}

/*set RAM*/
int intc_sram(const intcode *const vm, INTC_WORD address, INTC_WORD value) {
    if (address < 0 || address >= vm->ramsize) {
        return IC_SEGFAULT;
    }

    vm->ram[address] = value;
    return 0;
}

/*set IP*/
int intc_sip(intcode *const vm, INTC_WORD value) {
    if (value < 0 || value >= vm->ramsize) {
        return IC_SEGFAULT;
    }

    vm->ip = value;
    return 0;
}

int intc_oadd(intcode *const vm) {
    INTC_WORD op1, op2, op3, sum1, sum2, ip;
    int retcode = 0;

    ip = vm->ip;
    retcode =
        /* get values in ram */
        intc_gram(vm, ip + 1, &op1) || intc_gram(vm, ip + 2, &op2) ||
        intc_gram(vm, ip + 3, &op3) ||
        /* lookup indirections */
        intc_gram(vm, op1, &sum1) || intc_gram(vm, op2, &sum2) ||
        /* store result */
        intc_sram(vm, op3, sum1 + sum2) ||
        /* set ip forward */
        intc_sip(vm, ip + 4);

    if (retcode) {
        return retcode;
    }

    return 0;
}

int intc_omul(intcode *const vm) {
    INTC_WORD op1, op2, op3, sum1, sum2, ip;
    int retcode = 0;

    ip = vm->ip;
    retcode =
        /* get values in ram */
        intc_gram(vm, ip + 1, &op1) || intc_gram(vm, ip + 2, &op2) ||
        intc_gram(vm, ip + 3, &op3) ||
        /* lookup indirections */
        intc_gram(vm, op1, &sum1) || intc_gram(vm, op2, &sum2) ||
        /* store result */
        intc_sram(vm, op3, sum1 * sum2) ||
        /* set ip forward */
        intc_sip(vm, ip + 4);

    if (retcode) {
        return retcode;
    }

    return 0;
}

int intc_run(intcode *const vm) {
    enum opcodes instr;
    INTC_WORD buffer;

    do {
        if (intc_gram(vm, vm->ip, &buffer)) {
            return IC_SEGFAULT;
        }

        if(buffer>INT_MAX || buffer < INT_MIN){
            return IC_BAD_OPCODE;
        }

        instr = buffer;

        switch (instr) {
        case OPC_ADD:
            intc_oadd(vm);
            break;
        case OPC_MUL:
            intc_omul(vm);
            break;
        case OPC_STP:
            break;
        default:
            return IC_BAD_OPCODE;
        }
    } while (instr != OPC_STP);

    return 0;
}
