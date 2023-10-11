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

    VM_Proc(argv[1]);

    return 0;
}
