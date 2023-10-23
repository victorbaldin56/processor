#ifndef CODECTOR
#define CODECTOR

#include <stddef.h>

enum Masks {
    CMD = 0x0F,
    IMM = 0x20,
    REG = 0x30,
	RAM = 0x40,
};

typedef struct {
    unsigned char *code;
    size_t size;
} Code;

#define CODE_ASSERT(codearr)    \
    assert(codearr);            \
    assert(codearr->code);      \

Code *CodeCtor(Code *codearr, size_t code_size);

void CodeDtor(Code *codearr);

Code *CodeRealloc(Code *codearr, size_t coeff);

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

void CodeDump(const Code *codearr, Caller *caller);

#endif
