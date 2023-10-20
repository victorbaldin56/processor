#ifndef VM
#define VM

#include <stdio.h>
#include "stack.h"

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
	JMP  = 0x09,
	JA   = 0x0A,
	JB   = 0x0B,
	JE   = 0x0C,
	JAE  = 0x0D,
	JBE  = 0x0E,
};

enum Masks {
    CMD = 0x0F,
    IMM = 0x20,
    REG = 0x30,
	RAM = 0x40,
};

typedef struct {
    double *code;
    ssize_t size;
} Code;

typedef struct {
	Stack stack;
	double regs[NUM_REGS] = {};
} CPU;

Code *CodeCtor(Code *code_array, ssize_t code_size);

void CodeDtor(Code *code_array);

CPU *CPU_Ctor(CPU *cpu);

void CPU_Dtor(CPU *cpu);

void VM_Proc(char *filename);

#endif // VM
