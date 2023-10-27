#include <stdio.h>
#include <ctype.h>
#include "VM.h"
#include "colors.h"
#include "codector.h"
#include <assert.h>
#include "codedump.h"

void CodeDump(const Code *codearr, Caller *caller) {
    assert(caller && caller->func && caller->filename);

    fprintf(stderr, FG_GREEN);

    fprintf(stderr, "Code array dump: function %s() from file %s, line %zu:\n",
                     caller->func, caller->filename, caller->line);

    if (!codearr) {
        fprintf(stderr, FG_RED);
        fprintf(stderr, "CODEARR POINTER IS NULL!\n");
        fprintf(stderr, RESET);
        return;
    }

    fprintf(stderr, "code = [%p]\n", codearr->code);
    fprintf(stderr, "size = %zu\n", codearr->size);
    fprintf(stderr, "data: \n");

    if (!codearr->code) {
        fprintf(stderr, FG_RED);
        fprintf(stderr, "CODE DATA POINTER IS NULL!\n");
        fprintf(stderr, RESET);
        return;
    }

    for (size_t ip = 0; ip < codearr->size; ip += BYTES_PER_ROW) {
        fprintf(stderr, "%08zx: ", ip);

        for (size_t bc = 0; bc < BYTES_PER_ROW; bc++) {
            size_t offset = ip + bc;

            if (offset >= codearr->size) break;

            fprintf(stderr, "%02hhx ", codearr->code[ip + bc]);
        }

        fprintf(stderr, " |");

        for (size_t bc = 0; bc < BYTES_PER_ROW; bc++) {
            size_t offset = ip + bc;

            if (offset >= codearr->size) break;

            CHAR_DUMP(codearr->code[ip + bc], stderr);
        }

        fprintf(stderr, "|\n");
    }

    fprintf(stderr, RESET);
}
