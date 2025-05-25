#ifndef TOKENIZE_H
#define TOKENIZE_H

typedef struct {
    char value;
} BfToken;

typedef struct {
    char    *buff;
    int      current;
    int      line;

    int      token_count;
    int      token_capacity;
    char    *file;
    BfToken *tokens;
} BfLexer;

BfLexer *init_lexer(char *buff, char* file);
void *free_lexer(BfLexer *lexer);

void tokenize(BfLexer *lexer);

#endif