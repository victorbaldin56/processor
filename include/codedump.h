#ifndef CODEDUMP
#define CODEDUMP

#include <stdio.h>
#include <ctype.h>
#include "codector.h"
#include "VM.h"

const size_t BYTES_PER_ROW = 16; ///< number of bytes in a single row in code array dump

typedef struct {
    const char *func;
    const char *filename;
    size_t line;
} Caller;

#ifdef DEBUG

#define CODE_DUMP(codearr)                          \
    Caller caller = {__func__, __FILE__, __LINE__}; \
    CodeDump(codearr, &caller);

#else
#define CODE_DUMP(codearr) ;
#endif

#define CHAR_DUMP(chr, fp)                          \
    if (isprint(chr)) fputc(chr, fp);               \
    else              fputc('.', fp);

void CodeDump(const Code *codearr, Caller *caller);

#endif
