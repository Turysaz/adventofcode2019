/*
 *  AOC 2019 - 01
 */


#include <stdio.h>
#include <stdlib.h>

#define BUFSIZE 64

/*
Turn on compilation for part 2 by uncommenting or by compiling with '-D PART_2'.
#define PART_2
*/
#define DEBUG

/* Calculate for singe mass */
unsigned long calculate_fuel_simple(unsigned long mass) {
    unsigned long fuel;
    fuel = (mass / 3) - 2;
    return fuel < 0 ? 0 : fuel;
}

/* Calculate for fuel also */
unsigned long calculate_fuel_recursively(unsigned long mass) {
    unsigned long required;

    if (mass < 9) {
        return 0;
    }

    required = calculate_fuel_simple(mass);
    return required + calculate_fuel_recursively(required);
}

unsigned long calculate_fuel(unsigned long mass) {
#ifdef PART_2
    return calculate_fuel_recursively(mass);
#else
    return calculate_fuel_simple(mass);
#endif
}

int calculate_from_file(void) {
    FILE *fp;
    char buffer[BUFSIZE];
    unsigned long mass = 0, fuel = 0, total = 0;

    /* test */
    fp = fopen("../inputs/day01.txt", "r");
    if (ferror(fp)) {
        fclose(fp);
        return -1;
    }

    while (!ferror(fp) && fgets(buffer, BUFSIZE, fp)) {
        mass = atol(buffer);
        fuel = calculate_fuel(mass);
        total += fuel;
#ifdef DEBUG
        printf("%6lu: %6lu %10lu\n", mass, fuel, total);
#endif
    }

    fclose(fp);

    /* print result */
    printf("\n%lu\n", total);

    return 0;
}

int main() {
#ifdef DEBUG
    printf("12: %lu\n", calculate_fuel(12));
    printf("14: %lu\n", calculate_fuel(14));
    printf("1969: %lu\n", calculate_fuel(1969));
    printf("100756: %lu\n", calculate_fuel(100756));
    puts("\n");
#endif

    return calculate_from_file();
}
