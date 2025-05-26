#ifndef ANALYZE_H
#define ANALYZE_H

#include "parse.h"

typedef struct {
    BfAst *ast;
    char  *file;
    int    current;
} BfAnalyzer;

BfAnalyzer *init_analyzer(BfAst *ast, char *file);
void free_analyzer(BfAnalyzer *analyzer);

void analyze(BfAnalyzer *parser);

#endif