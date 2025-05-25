#include <stdlib.h>
#include <stdio.h>

#include "tokenize.h"

BfLexer *init_lexer(char *buff, char* file) {
    BfLexer *lexer = malloc(sizeof(BfLexer));
    lexer->current = 0;
    lexer->buff = buff;
    lexer->file = file;
    lexer->line = 1;
    lexer->token_count = 0;
    lexer->token_capacity = 1;
    lexer->tokens = malloc(sizeof(BfToken));

    return lexer;
}

void *free_lexer(BfLexer *lexer) {
    free(lexer->tokens);
    free(lexer);
}

static int is_valid_token(char c) {
    return c == '+'
        || c == '-'
        || c == '<'
        || c == '>'
        || c == '.'
        || c == ','
        || c == '['
        || c == ']';
}

static inline char current(BfLexer *lexer) {
    return lexer->buff[lexer->current];
}

void tokenize(BfLexer *lexer) {
    while (current(lexer)) {
        while (current(lexer) == ' ' || current(lexer) == '\r' || current(lexer) == '\n') {
            if (current(lexer) == '\n') {
                lexer->line++;
            }

            lexer->current++;
        }

        if (!is_valid_token(current(lexer))) {
            printf("%s:%d invalid token '%c'\n", lexer->file, lexer->line, current(lexer));
            lexer->current++;
            continue;
        }

        BfToken token;
        token.value = current(lexer);

        if (lexer->token_count >= lexer->token_capacity) {
            lexer->token_capacity *= 2;
            lexer->tokens = realloc(lexer->tokens, sizeof(BfToken) * lexer->token_capacity);
        }

        lexer->tokens[lexer->token_count++] = token;
        lexer->current++;
    }

    for (int i = 0; i < lexer->token_count; i++) {
        printf("Token: %c\n", lexer->tokens[i].value);
    }
}
