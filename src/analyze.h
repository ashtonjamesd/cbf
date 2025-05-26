#ifndef ANALYZE_H
#define ANALYZE_H

#include "parse.h"

typedef struct {
    BfAst *ast;
} BfAnalyzer;

void analyze(BfParser *parser);

#endif