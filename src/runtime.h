#ifndef RUNTIME_H
#define RUNTIME_H

#include <stdint.h>
#include "parse.h"

typedef struct {
    BfAst *ast;
    int count;

    uint8_t *tape;
    int tape_size;
    int ptr;
} BfRuntime;

BfRuntime *init_runtime(BfAst *ast, int count);
void execute(BfRuntime *runtime);

#endif