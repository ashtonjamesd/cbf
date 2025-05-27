#include <stdio.h>
#include <stdlib.h>
#include "x86.h"

static int loop_id = 0;

static void generate_node(FILE *fptr, BfAstNode node) {
    switch (node.type) {
        case BF_INC_PTR:
            fprintf(fptr, "\tadd rsi, %d\n", node.count);
            break;

        case BF_DEC_PTR:
            fprintf(fptr, "\tsub rsi, %d\n", node.count);
            break;

        case BF_INC_DATA:
            fprintf(fptr, "\tadd byte [rsi], %d\n", node.count);
            break;

        case BF_DEC_DATA:
            fprintf(fptr, "\tsub byte [rsi], %d\n", node.count);
            break;

        case BF_INPUT:
            fprintf(
                fptr,
                "\tmov rax, 0\n"
                "\tmov rdi, 0\n"
                "\tmov rdx, 1\n"
                "\tsyscall\n"
            );
            break;

        case BF_OUTPUT:
            for (int i = 0; i < node.count; i++) {
                fprintf(
                    fptr,
                    "\tmov rax, 1\n"
                    "\tmov rdi, 1\n"
                    "\tmov rdx, 1\n"
                    "\tsyscall\n"
                );
            }
            break;

        case BF_LOOP: {
            int id = loop_id++;
            fprintf(fptr, "loop_start_%d:\n", id);
            fprintf(fptr, "\tcmp byte [rsi], 0\n");
            fprintf(fptr, "\tje loop_end_%d\n", id);

            for (int i = 0; i < node.loop.count; i++) {
                generate_node(fptr, node.loop.body[i]);
            }

            fprintf(fptr, "\tjmp loop_start_%d\n", id);
            fprintf(fptr, "loop_end_%d:\n", id);
            break;
        }

        default:
            break;
    }
}

void generate_x86(BfAst *ast) {
    FILE *fptr = fopen("out.asm", "w");
    fprintf(
        fptr,
        "section .bss\n"
        "\ttape resb 30000\n\n"
        "section .text\n"
        "\tglobal _start\n"
        "\n_start:\n"
        "\tmov rsi, tape\n"
    );

    for (int i = 0; i < ast->count; i++) {
        generate_node(fptr, ast->nodes[i]);
    }

    fprintf(
        fptr,
        "\tmov rax, 60\n"
        "\tmov rdi, 0\n"
        "\tsyscall\n"
    );

    fclose(fptr);

    system("nasm -f elf64 out.asm -o out.o");
    system("ld out.o -o out");
    system("./out");

    system("rm out.asm");
    system("rm out.o");
}