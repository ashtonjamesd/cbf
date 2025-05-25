#include <stdio.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        perror("Usage: cbf *.bfq");
        return 1;
    }

    FILE *fptr = fopen("example/source.bf", "r");

    printf("Hello, World!\n");
    return 0; 
}