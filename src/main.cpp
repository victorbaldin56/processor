#include <stdio.h>
#include <string.h>
#include "stack.h"
#include "colors.h"
#include "VM.h"

const double VERSION = 1.0;

int main(int argc, char *argv[]) {
    if (argc == 1) {
        printf(FG_RED);
        printf("Processor: no program to execute\n");
        printf(RESET);
        return -1;
    }

    if (!strcmp(argv[1], "--help")) {
        printf("This is processor emulator, version %.1lf\n", VERSION);
        printf("(c) Victor Baldin, 2023\n");
        printf("Usage: processor [options] [filename]\n");
        printf("Options:\n");
        printf("\t--help %50s"   , "print this message and exit\n");
        printf("\t--version %50s", "version\n");
        return 0;
    }

    if (!strcmp(argv[1], "--version")) {
        printf("Version %.1lf\n", VERSION);
        return 0;
    }

    if (Process(argv[1]) != 0) {
        perror("Processor");
        return -1;
    }

    return 0;
}
