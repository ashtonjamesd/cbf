#include <stdio.h>
#include <stdlib.h>

#include "tokenize.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        perror("Usage: cbf *.bf");
        return 1;
    }

    FILE *fptr = fopen(argv[1], "r");
    fseek(fptr, 0, SEEK_END);
    long sz = ftell(fptr);
    rewind(fptr);

    char *buff = malloc(sz + 1);
    fread(buff, 1, sz, fptr);
    buff[sz] = '\0';

    BfLexer *lexer = init_lexer(buff, argv[1]);
    tokenize(lexer);

    free_lexer(lexer);

    free(buff);
    fclose(fptr);

    return 0; 
}