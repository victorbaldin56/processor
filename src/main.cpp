#include "stack.h"
#include "colors.h"
#include "../include/VM.h"

int main(int argc, char *argv[]) {
    if (argc == 1) {
        printf(FG_RED);
        printf("VM_CPU: no program to execute\n");
        printf(RESET);
        return -1;
    }

    FILE *input_file = fopen(argv[1], "r"); // open a file with bytecode

    if (!input_file) {
        perror("VM_CPU");
        return -1;
    }

    VM_Proc(input_file);

    fclose(input_file);
    input_file = NULL;

    return 0;
}
