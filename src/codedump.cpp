#include <stdio.h>
#include <ctype.h>
#include <time.h>
#include <limits.h>
#include <stdlib.h>
#include "VM.h"
#include "colors.h"
#include "codector.h"
#include <assert.h>
#include "codedump.h"

static FILE *creat_log_file();

void CodeDump(const Code *codearr, Caller *caller) {
    assert(caller && caller->func && caller->filename);

    FILE *log_file_ptr = creat_log_file();
    assert(log_file_ptr);

    fprintf(log_file_ptr, "Code array dump: function %s() from file %s, line %zu:\n",
                     caller->func, caller->filename, caller->line);

    if (!codearr) {
        fprintf(log_file_ptr, "CODEARR POINTER IS NULL!\n");
        return;
    }

    fprintf(log_file_ptr, "code = [%p]\n", codearr->code);
    fprintf(log_file_ptr, "size = %zu\n", codearr->size);
    fprintf(log_file_ptr, "data: \n");

    if (!codearr->code) {
        fprintf(log_file_ptr, "CODE DATA POINTER IS NULL!\n");
        return;
    }

    fprintf(log_file_ptr, "          00 01 02 03 04 05 06 07 08 09 0a 0b 0c 0d 0e 0f\n");

    for (size_t ip = 0; ip < codearr->size; ip += BYTES_PER_ROW) {
        fprintf(log_file_ptr, "%08zx: ", ip);

        for (size_t bc = 0; bc < BYTES_PER_ROW; bc++) {
            size_t offset = ip + bc;

            if (offset >= codearr->size) break;

            fprintf(log_file_ptr, "%02hhx ", codearr->code[ip + bc]);
        }

        fprintf(log_file_ptr, " |");

        for (size_t bc = 0; bc < BYTES_PER_ROW; bc++) {
            size_t offset = ip + bc;

            if (offset >= codearr->size) break;

            CHAR_DUMP(codearr->code[ip + bc], log_file_ptr);
        }

        fprintf(log_file_ptr, "|\n");
    }
}

static FILE *creat_log_file() {
    system("mkdir -p logs\n");
    time_t date = time(NULL);

    char filename[FILENAME_MAX] = {};

    strftime(filename, sizeof(filename), "logs/code_dump_%Y-%m-%d_%H:%M:%S.log", gmtime(&date));

    return fopen(filename, "w");
}
