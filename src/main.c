#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tokenize.h"
#include "parse.h"
#include "analyze.h"
#include "utils.h"
#include "runtime.h"

#define IS_RUNTIME(x) strcmp(argv[i], x) == 0

int main(int argc, char *argv[]) {
    if (argc < 2) {
        perror("Usage: cbf *.bf");
        return 1;
    }

    char *runtime = "-interpret";
    int debug = 0;
    for (int i = 1; i < argc; i++) {
        if (IS_RUNTIME("-x86") || IS_RUNTIME("-interpret")) {
            runtime = argv[i];
        }
        else if (strcmp("-d", argv[i])) {
            debug = 1;
        }
    }

    FILE *fptr = fopen(argv[1], "r");
    if (!fptr) {
        perror("Error opening file");
        return 1;
    }

    fseek(fptr, 0, SEEK_END);
    long sz = ftell(fptr);
    rewind(fptr);

    char *buff = malloc(sz + 1);
    if (!buff) {
        perror("Error allocating for source file");
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
    else {
        printf("runtime '%s' not supported\n", runtime);
    }

    free_parser(parser);

    free(buff);
    fclose(fptr);

    return 0; 
}