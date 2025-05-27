#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tokenize.h"
#include "parse.h"
#include "analyze.h"
#include "utils.h"
#include "runtime.h"
#include "x86.h"

#define match(x) strcmp(argv[i], x) == 0

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("\nUsage: cbf *.bf\n");
        return 1;
    }

    char *runtime = "-interpret";
    int debug = 0;
    for (int i = 2; i < argc; i++) {
        if (match("-x86") || match("-interpret")) {
            runtime = argv[i];
        }
        else if (match("-d")) {
            debug = 1;
        }
        else {
            printf("\nunknown flag '%s'\n", argv[i]);
            return 1;
        }
    }

    FILE *fptr = fopen(argv[1], "r");
    if (!fptr) {
        perror("Error opening file\n");
        return 1;
    }

    fseek(fptr, 0, SEEK_END);
    long sz = ftell(fptr);
    rewind(fptr);

    char *buff = malloc(sz + 1);
    if (!buff) {
        perror("Error allocating for source file\n");
        fclose(fptr);
        return 1;
    }
    fread(buff, 1, sz, fptr);
    buff[sz] = '\0';

    BfLexer *lexer = init_lexer(buff, argv[1]);
    tokenize(lexer);

    if (debug) print_lexer(lexer);

    BfParser *parser = init_parser(lexer->tokens, lexer->token_count, lexer->file);
    parse(parser);

    free_lexer(lexer);

    if (debug) print_ast(parser->ast);

    BfAnalyzer *analyzer= init_analyzer(parser->ast, parser->file);
    analyze(analyzer);
    free_analyzer(analyzer);

    if (strcmp(runtime, "-interpret") == 0) {
        BfRuntime *runtime = init_runtime(parser->ast, parser->ast->count);
        execute(runtime);
        free_runtime(runtime);
    }
    else if (strcmp(runtime, "-x86") == 0) {
        generate_x86(parser->ast);
    }
    else {
        printf("runtime '%s' not supported\n", runtime);
    }

    free_parser(parser);

    free(buff);
    fclose(fptr);

    return 0; 
}