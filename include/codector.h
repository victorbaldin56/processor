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

#endif
