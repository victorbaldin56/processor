#ifndef CODECTOR
#define CODECTOR

#include <stddef.h>
#include <inttypes.h>

const size_t NUM_REGS = 5;    ///< number of registers (including tmp)

const int32_t SIGNATURE  = 0x56414223;

const unsigned char VERSION = 0x11;    ///< format version

const char BIGVERS = 0xF0;

const char LITVERS = 0x0F;

const size_t SIGNATURE_SIZE = sizeof(int32_t) + 1;

const size_t RAM_SIZE = 1024; ///< RAM size (in doubles)

#define DEBUG

#ifdef DEBUG
#define ON_DEBUG(...) __VA_ARGS__
#else
#define ON_DEBUG(...) ;
#endif

enum Masks {
    CMD = 0x1F,
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
