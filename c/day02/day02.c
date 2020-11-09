
#include "intcode.h"
#include <stdio.h>

int part1(/*argc, argv*/) {
    intcode vm;
    int retc;

    retc = intc_init("../input/day02.txt", &vm);

    if (retc) {
        printf("VM init fail, retcode %i\n", retc);
        return -1;
    }

    /* restore error code */
    vm.ram[1] = 12;
    vm.ram[2] = 2;

    retc = intc_run(&vm);
    if (retc) {
        printf("VM1 run fail, retcode %i\n", retc);
        intc_dtor(&vm);
        return -1;
    }

    printf("%li\n", vm.ram[0]);
    intc_dtor(&vm);

    return 0;
}

int part2(/*argc, argv*/) {
    intcode vm;
    intcode copy;
    INTC_WORD noun, verb;
    int retc = 0;
    int input_word;
    int success = 0;

    retc = intc_init("../input/day02.txt", &vm);

    if (retc) {
        printf("VM init fail, retcode %i\n", retc);
        return -1;
    }

    for (input_word = 0; input_word < 9999; input_word++) {
        retc = intc_copy(&vm, &copy);

        if (retc) {
            printf("VM init fail for copy %i, retcode %i\n", input_word, retc);
            return -1;
        }

        noun = input_word / 100;
        verb = input_word - noun * 100;

        /* restore error code */
        copy.ram[1] = noun;
        copy.ram[2] = verb;

        /*printf("%li, %li\n", noun, verb);*/

        retc = intc_run(&copy);

        if (retc) {
            printf("config %i failed, retcode %i\n", input_word, retc);
            intc_dtor(&copy);
            continue;
        }

        if (copy.ram[0] == 19690720)
        {
            intc_dtor(&copy);
            success = 1;
            break;
        }

        intc_dtor(&copy);
    }

    intc_dtor(&vm);
    if(success)
    {
        printf("Configuration: %i\n", input_word);
    }else {
        printf("The requested output could not be achieved.\n");
    }

    return 0;
}

int main() {
    part2();
}
