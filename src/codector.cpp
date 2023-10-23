#include <stdio.h>
#include <stdlib.h>
#include "codector.h"
#include <assert.h>

Code *CodeCtor(Code *codearr, size_t code_size) {
    assert(codearr);

    codearr->size = code_size;
    codearr->code = (unsigned char *)calloc(code_size, sizeof(double));

    if (!codearr->code) return NULL;

    return codearr;
}

void CodeDtor(Code *codearr) {
    CODE_ASSERT(codearr);

    free(codearr->code);
    codearr->code = NULL;
    codearr->size = 0;
}

Code *CodeRealloc(Code *codearr, size_t coeff) {
    CODE_ASSERT(codearr);

    unsigned char *newcode = (unsigned char *)realloc(codearr->code, codearr->size * coeff);

    if (!newcode) return NULL;

    codearr->code = newcode;
    codearr->size *= coeff;

    return codearr;
}

void CodeDump(const Code *codearr, Caller *caller) {
    assert(caller && caller->func && caller->filename);

    fprintf(stderr, "Code array dump: function %s() from file %s, line %zu:\n",
                     caller->func, caller->filename, caller->line);

    if (!codearr) {
        fprintf(stderr, "CODEARR POINTER IS NULL!\n");
        return;
    }

    fprintf(stderr, "code = [%p]\n", codearr->code);
    fprintf(stderr, "size = %zu\n", codearr->size);
    fprintf(stderr, "data: \n");

    if (!codearr->code) {
        fprintf(stderr, "CODE DATA POINTER IS NULL!\n");
        return;
    }

    for (size_t ip = 0; ip < codearr->size; ip += BYTES_PER_ROW) {
        fprintf(stderr, "%8zx: ", ip);

        for (size_t bc = 0; bc < BYTES_PER_ROW; bc++) {
            fprintf(stderr, "%2hhx ", codearr->code[ip * BYTES_PER_ROW + bc]);
        }

        fprintf(stderr, " |");

        for (size_t bc = 0; bc < BYTES_PER_ROW; bc++) {
            fputc(codearr->code[ip * BYTES_PER_ROW + bc], stderr);
        }

        fprintf(stderr, "|\n");
    }
}
