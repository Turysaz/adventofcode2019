
#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "read.h"

enum readerr { FERR_OPEN = -1, FERR_RD = -2 };

long frdall(const char *fname, char **result) {

    /* file content buffer */
    char *buffer = NULL;
    long length = 0;
    long readc = 0;
    int retcode = 0;

    FILE *f = fopen(fname, "rb");

    if (!f) {
        retcode = FERR_OPEN;
        goto clean;
    }

    /* set stream pointer to end*/
    fseek(f, 0, SEEK_END);
    /* get current position */
    length = ftell(f);
    /* reset pointer to beginning*/
    fseek(f, 0, SEEK_SET);

    /* add 1 to add \0 to the end. */
    buffer = malloc(length + 1);

    if (!buffer) {
        retcode = MALLOC_ERR;
        goto clean;
    }

    readc = fread(buffer, /* read to buffer */
                  1,      /* objects of size 1 (char) */
                  length /* so many of them: whole file */, f);

    if (ferror(f) || (readc != length && !feof(f))) {
        retcode = FERR_RD;
        goto clean;
    }

    buffer[length + 1] = '\0';

clean:
    if (f) {
        fclose(f);
        f = NULL;
    }

    if (retcode < 0 && buffer) {
        free(buffer);
        buffer = NULL;
    }

    (*result) = buffer;
    return retcode < 0 ? retcode : length + 1;
}
