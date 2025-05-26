#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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

void free_runtime(BfRuntime *runtime) {
    free(runtime->tape);
    free(runtime);
}

int execute_instr(BfAstNode node, BfRuntime *runtime) {
    switch (node.type) {
        case BF_INC_DATA:
            runtime->tape[runtime->ptr] += node.count;
            break;
        case BF_DEC_DATA:
            runtime->tape[runtime->ptr] -= node.count;
            break;
        case BF_INC_PTR:
            runtime->ptr += node.count;
            if (runtime->ptr >= runtime->tape_size) {
                size_t new_size = runtime->ptr + 1;
                runtime->tape = realloc(runtime->tape, new_size * sizeof(uint8_t));
                memset(runtime->tape + runtime->tape_size, 0, (new_size - runtime->tape_size));
                runtime->tape_size = new_size;
            }
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
        default:
            printf("runtime error: unknown instruction");
            return 1;
    }

    return 0;
}

void execute(BfRuntime *runtime) {
    for (int i = 0; i < runtime->count; i++) {
        int err = execute_instr(runtime->ast->nodes[i], runtime);
        if (err) break;
    }
}