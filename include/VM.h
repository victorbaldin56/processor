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

enum Commands {
    HALT = 0x0F,
	IN =   0x00,
	OUT =  0x01,
	PUSH = 0x02,
	ADD  = 0x03,
	SUB  = 0x04,
	MULT = 0x05,
	DIV  = 0x06,
	SQRT = 0x07,
	POP  = 0x08,
};

enum Flags {
    CMD = 0x0F,
    IMM = 0x10,
    REG = 0x20,
};

typedef struct {
    double *code;
    ssize_t size;
} Code;

void VM_Proc(char *filename);

#endif // VM
