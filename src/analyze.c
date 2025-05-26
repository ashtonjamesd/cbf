#include <stdio.h>
#include <stdarg.h>

#include "analyze.h"

BfOpType opposite_instr(BfAstNode node) {
    switch (node.type) {
        case BF_INC_DATA: return BF_DEC_DATA;
        case BF_DEC_DATA: return BF_INC_DATA;
        case BF_INC_PTR:  return BF_DEC_PTR;
        case BF_DEC_PTR:  return BF_INC_PTR;
        default:          return BF_BAD;
    }
}

static void bf_warning(BfParser *parser, BfAstNode node, char *fmt, ...) {
    if (node.start_line != node.end_line) {
        printf("\n%s: expression from line %d to %d", parser->file, node.start_line, node.end_line);
    } else {
        printf("\n%s: expression on line %d", parser->file, node.start_line);
    }

    printf("\nwarning: ");

    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);

    printf("\n");
}

void analyze_node(BfParser *parser, BfAstNode node, int *i) {
    if (node.type == BF_INC_DATA || node.type == BF_DEC_DATA ||
        node.type == BF_INC_PTR  || node.type == BF_DEC_PTR) {
        BfAstNode next = parser->ast->nodes[*i + 1];

        if (next.type == opposite_instr(node)) {
            *i += 1;
            bf_warning(parser, node, "redundant instructions '%s' and '%s' cancel each other out", node.c, next.c);
        }
    }

    if (node.type == BF_LOOP && node.loop.count == 0) {
        bf_warning(parser, node, "empty loop");
    } else {
        for (int j = 0; j < node.loop.count; j++) {
            printf("yep");
            analyze_node(parser, node.loop.body[j], i);
        }
    }
}

void analyze(BfParser *parser) {
    for (int i = 0; i < parser->ast->count; i++) {
        BfAstNode node = parser->ast->nodes[i];
        analyze_node(parser, node, &i);
    }
}