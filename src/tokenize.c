#include <stdlib.h>
#include <stdio.h>

#include "tokenize.h"

void print_lexer(BfLexer *lexer) {
    for (int i = 0; i < lexer->token_count; i++) {
        printf("Token: %c\n", lexer->tokens[i].value);
    }
}

BfLexer *init_lexer(char *buff, char* file) {
    BfLexer *lexer = malloc(sizeof(BfLexer));
    lexer->current = 0;
    lexer->buff = buff;
    lexer->file = file;
    lexer->line = 1;
    lexer->column = 1;
    lexer->token_count = 0;
    lexer->token_capacity = 16;
    lexer->tokens = malloc(sizeof(BfToken) * lexer->token_capacity);

    return lexer;
}

void free_lexer(BfLexer *lexer) {
    free(lexer->tokens);
    free(lexer);
}

static inline char current(BfLexer *lexer) {
    return lexer->buff[lexer->current];
}

void tokenize(BfLexer *lexer) {
    int loop_start = 0;
    int loop_end = 0;

    while (current(lexer)) {
        while (current(lexer) == ' ' || current(lexer) == '\r' || current(lexer) == '\n') {
            if (current(lexer) == '\n') {
                lexer->line++;
                lexer->column = 1;
            } else {
                lexer->column++;
            }

            lexer->current++;
        }

        BfToken token;
        token.value = current(lexer);
        token.line = lexer->line;
        token.column = lexer->column;

        if (token.value == '[') {
            loop_start++;
        } else if (token.value == ']') {
            loop_end++;
        }

        if (lexer->token_count >= lexer->token_capacity) {
            lexer->token_capacity *= 2;
            lexer->tokens = realloc(lexer->tokens, sizeof(BfToken) * lexer->token_capacity);
        }

        lexer->tokens[lexer->token_count++] = token;
        lexer->current++;
        lexer->column++;
    }

    if (loop_start != loop_end) {
        printf("\n%s:%d:%d: error: unterminated loops\n", lexer->file, lexer->line, lexer->column);
    }
}