#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#include "analyze.h"

BfAnalyzer *init_analyzer(BfAst *ast, char *file) {
    BfAnalyzer *analyzer = malloc(sizeof(BfAnalyzer));
    analyzer->ast = ast;
    analyzer->file = file;
    analyzer->current = 0;

    return analyzer;
}

BfOpType opposite_instr(BfAstNode node) {
    switch (node.type) {
        case BF_INC_DATA: return BF_DEC_DATA;
        case BF_DEC_DATA: return BF_INC_DATA;
        case BF_INC_PTR:  return BF_DEC_PTR;
        case BF_DEC_PTR:  return BF_INC_PTR;
        default:          return BF_BAD;
    }
}

static void warn(BfAnalyzer *analyzer, BfAstNode node, char *fmt, ...) {
    if (node.start_line != node.end_line) {
        printf("\n%s: expression from line %d to %d", analyzer->file, node.start_line, node.end_line);
    } else {
        printf("\n%s: expression on line %d", analyzer->file, node.start_line);
    }

    printf("\nwarning: ");

    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);

    printf("\n");
}

void analyze_node(BfAnalyzer *analyzer, BfAstNode *nodes, int count) {
    for (int i = 0; i < count; i++) {
        BfAstNode node = nodes[i];

        if (node.type == BF_INC_DATA || node.type == BF_DEC_DATA || node.type == BF_INC_PTR  || node.type == BF_DEC_PTR) {
            if (i + 1 < count) {
                BfAstNode next = nodes[i + 1];

                if (next.type == opposite_instr(node)) {
                    analyzer->current++;
                    warn(analyzer, node, "redundant instructions '%s' and '%s' cancel each other out", node.c, next.c);
                }
            }
        }

        if (node.type == BF_LOOP) {
            if (node.loop.count == 0) {
                warn(analyzer, node, "empty loop");
            } else {
                analyze_node(analyzer, node.loop.body, node.loop.count);
            }
        }
    }
}

void analyze(BfAnalyzer *analyzer) {
    analyze_node(analyzer, analyzer->ast->nodes, analyzer->ast->count);
}