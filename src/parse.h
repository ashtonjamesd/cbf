#ifndef PARSE_H
#define PARSE_H

#include "tokenize.h"
#include "common.h"

typedef struct BfAstNode BfAstNode;

struct BfAstNode {
    BfOpType type;
    char    *c;

    int      start_line;
    int      end_line;
    int      start_col;
    int      end_col;

    union {
        int count;
        struct {
            BfAstNode *body;
            int        count;
            int        capacity;
        } loop;
    };
};

typedef struct {
    BfAstNode *nodes;
    int        count;
    int        capacity;
} BfAst;

typedef struct {
    int      current;
    BfToken *tokens;
    int      token_count;
    BfAst   *ast;
    char    *file;
} BfParser;

BfParser *init_parser(BfToken *tokens, int count, char *file);
void parse(BfParser *parser);

#endif