#ifndef CODECTOR
#define CODECTOR

#include <stddef.h>

const size_t NUM_REGS = 4; ///< number of registers

#define DEBUG

#ifdef DEBUG
#define ON_DEBUG(x) x
#else
#define ON_DEBUG(x)
#endif

enum Masks {
    CMD = 0x0F,
    IMM = 0x20,
    REG = 0x40,
	RAM = 0x80,
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
