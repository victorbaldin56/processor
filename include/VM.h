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

typedef struct {
	Stack stack;
	double regs[NUM_REGS] = {};
} CPU;

CPU *CPU_Ctor(CPU *cpu);

void CPU_Dtor(CPU *cpu);

void VM_Proc(char *filename);

#endif // VM
