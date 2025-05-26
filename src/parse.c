#include <stdlib.h>
#include <stdio.h>

#include "parse.h"
#include "utils.h"

static BfAstNode parse_node(BfParser *parser);

BfParser *init_parser(BfToken *tokens, int token_count, char *file) {
    BfParser *parser = malloc(sizeof(BfParser));
    parser->current = 0;
    parser->tokens = tokens;
    parser->token_count = token_count;
    parser->file = file;
    
    parser->ast = malloc(sizeof(BfAst));
    parser->ast->capacity = 16;
    parser->ast->count = 0;
    parser->ast->nodes = malloc(sizeof(BfAstNode) * parser->ast->capacity);

    return parser;
}

static BfOpType map_type(BfToken c) {
    switch (c.value) {
        case '+': return BF_INC_DATA;
        case '-': return BF_DEC_DATA;
        case '>': return BF_INC_PTR;
        case '<': return BF_DEC_PTR;
        case '.': return BF_OUTPUT;
        case ',': return BF_INPUT;
        case '[': return BF_LOOP;
        
        // do not account for this as this token will only be found when parsing a '['
        // case ']': return BF_LOOP;
    }

    // should never get here
    return BF_BAD;
}

static BfAstNode new_loop_node(struct BfAstNode *body, int count, int start_line, int end_line, int start_col, int end_col) {
    BfAstNode node;
    node.type = BF_LOOP;
    node.loop.count = count;
    node.loop.body = body;
    node.start_line = start_line;
    node.end_line = end_line;
    node.start_col = start_col;
    node.end_col = end_col;

    return node;
}
static BfAstNode new_node(BfOpType type, int count, int start_line, int end_line, int start_col, int end_col) {
    BfAstNode node;
    node.type = type;
    node.count = count;
    node.start_line = start_line;
    node.end_line = end_line;
    node.start_col = start_col;
    node.end_col = end_col;
    node.c = bf_op_type_to_str(type);

    return node;
}

static int is_end(BfParser *parser) {
    return parser->current >= parser->token_count;
}

static BfToken current(BfParser *parser) {
    return parser->tokens[parser->current];
}

static int match(char c, BfParser *parser) {
    if (is_end(parser)) return 0;
    return parser->tokens[parser->current].value == c;
}

static BfAstNode parse_primary_node(BfParser *parser) {
    int start = parser->current;

    BfToken token = current(parser);
    while (!is_end(parser) && match(token.value, parser)) {
        parser->current++;
    }

    int count = parser->current - start;
    int type = map_type(token);

    int start_line = parser->tokens[start].line;
    int end_line = parser->tokens[parser->current - 1].line;

    return new_node(type, count, start_line, end_line, -1, -1);
}

static BfAstNode parse_loop_node(BfParser *parser) {
    int start = parser->current;
    
    int capacity = 1;
    int count = 0;
    BfAstNode *body = malloc(sizeof(BfAstNode) * capacity);

    parser->current++;
    while (!is_end(parser) && !match(']', parser)) {
        BfAstNode node = parse_node(parser);

        if (count >= capacity) {
            capacity *= 2;
            body = realloc(body, capacity * sizeof(BfAstNode));
        }
        body[count++] = node;
    }
    parser->current++;

    int start_line = parser->tokens[start].line;
    int end_line = parser->tokens[parser->current - 1].line;

    return new_loop_node(body, count, start_line, end_line, -1, -1);
}

static BfAstNode parse_node(BfParser *parser) {
    if (is_end(parser)) {
        return new_node(BF_BAD, 0, -1, -1, -1, -1);
    }
    
    BfOpType type = map_type(current(parser));
    
    if (type == BF_LOOP) {
        return parse_loop_node(parser);
    } else {
        return parse_primary_node(parser);
    }
}

void parse(BfParser *parser) {
    while (!is_end(parser)) {
        BfAstNode node = parse_node(parser);
        if (node.type == BF_BAD) break;
        
        if (parser->ast->count >= parser->ast->capacity) {
            parser->ast->capacity *= 2;
            parser->ast->nodes = realloc(parser->ast->nodes, sizeof(BfAstNode) * parser->ast->capacity);
        }
        parser->ast->nodes[parser->ast->count++] = node;
    }
}