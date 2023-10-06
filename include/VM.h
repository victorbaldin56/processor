#ifndef VM
#define VM

#include <stdio.h>

#define DEBUG

#ifdef DEBUG
#define ON_DEBUG(x) x
#else
#define ON_DEBUG(x)
#endif

const size_t NUM_REGS = 4; ///< number of registers

typedef struct {
    unsigned char *code;
    size_t size;
} Code;

void VM_Proc(FILE *inpf);

#endif // VM
