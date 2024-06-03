#ifndef VASM
#define VASM

#include <stdio.h>
#include <limits.h>
#include "codector.h"

#define ASM_RAISE_ERR(msg)                                                          \
    fprintf(stderr, FG_RED);                                                        \
    fprintf(stderr, "error: file %s, line %zu: %s\n", inp_filename, lp, msg);       \
    fprintf(stderr, "  %zu | %s\n", lp, text->lines[lp]);                           \
    fprintf(stderr, RESET);                                                         \
    return ASSEMBLE_FAILURE;

enum CmdError {
    NO_ERR,
    NO_ARG,
    INCORRECT_ARG,
    UNKNOWN,
    EMIT_FAILURE,
    NUM_LABELS_EXCEED,
    LABEL_NOT_FOUND,
};

enum AsmError {
    SUCCESS,
    BUF_ALLOC_FAILURE,
    LINEARR_ALLOC_FAILURE,
    FILE_CREAT_FAILURE,
    ASSEMBLE_FAILURE,
};

const size_t NUM_LABELS = 256;                     ///< max number of labels

const size_t LABEL_LENGHT = 16;

typedef struct {
    char names[NUM_LABELS][LABEL_LENGHT] = {{}};
    double addrs[NUM_LABELS] = {};
    size_t count;
} Labels;

AsmError Assemble(const char *input, const char *output);

const size_t INIT_CODE_SIZE = 16;                   ///< initial size of code array

#endif // VASM
