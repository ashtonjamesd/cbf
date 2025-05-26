#include <stdio.h>
#include <stdlib.h>

#include "tokenize.h"
#include "parse.h"
#include "analyze.h"
#include "utils.h"

void print_ast_node(BfAstNode node, int indent) {
    for (int i = 0; i < indent; i++) {
        printf("  ");
    }

    if (node.type != BF_LOOP) {
        printf("(%d) %s\n", node.count, bf_op_type_to_str(node.type));
    } else {
        printf("Loop (count %d):\n", node.loop.count);
        for (int i = 0; i < node.loop.count; i++) {
            print_ast_node(node.loop.body[i], indent + 1);
        }
    }
}

void print_ast(BfAst *ast) {
    for (int i = 0; i < ast->count; i++) {
        print_ast_node(ast->nodes[i], 0);
    }
}

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

    // for (int i = 0; i < lexer->token_count; i++) {
    //     printf("Token: %c\n", lexer->tokens[i].value);
    // }

    BfParser *parser = init_parser(lexer->tokens, lexer->token_count, lexer->file);
    parse(parser);

    print_ast(parser->ast);

    analyze(parser);

    free_lexer(lexer);

    free(buff);
    fclose(fptr);

    return 0; 
}