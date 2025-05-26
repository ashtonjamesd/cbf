#include <stdlib.h>
#include <stdio.h>

#include "runtime.h"

BfRuntime *init_runtime(BfAst *ast, int count) {
    BfRuntime *runtime = malloc(sizeof(BfRuntime));
    runtime->ast = ast;
    runtime->count = count;
    runtime->ptr = 0;
    runtime->tape_size = 4;
    runtime->tape = calloc(runtime->tape_size, sizeof(uint8_t));

    return runtime;
}


void execute_instr(BfAstNode node, BfRuntime *runtime) {
    switch (node.type) {
        case BF_INC_DATA:
            runtime->tape[runtime->ptr] += node.count;
            break;
        case BF_DEC_DATA:
            runtime->tape[runtime->ptr] -= node.count;
            break;
        case BF_INC_PTR:
            runtime->ptr += node.count;
            break;
        case BF_DEC_PTR:
            runtime->ptr -= node.count;
            break;
        case BF_INPUT:
            for (int i = 0; i < node.count; i++) {
                runtime->tape[runtime->ptr] = getchar();
            }
            break;
        case BF_OUTPUT:
            for (int i = 0; i < node.count; i++) {
                putchar(runtime->tape[runtime->ptr]);
            }
            break;
        case BF_LOOP:
            while (runtime->tape[runtime->ptr] != 0) {
                for (int i = 0; i < node.loop.count; i++) {
                    execute_instr(node.loop.body[i], runtime);
                }
            }
            break;
    }
}

void execute(BfRuntime *runtime) {
    for (int i = 0; i < runtime->count; i++) {
        execute_instr(runtime->ast->nodes[i], runtime);
    }
}