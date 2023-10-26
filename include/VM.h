#ifndef VM
#define VM

#include <stdio.h>
#include "stack.h"
#include "codector.h"

#define DEBUG

typedef struct {
	Stack stack;
	double regs[NUM_REGS] = {};
	double RAM[RAM_SIZE]  = {};
} CPU;

#define CPU_ASSERT(cpu)			\
	assert(cpu);				\
	STACK_ASS(&cpu->stack);

CPU *CPU_Ctor(CPU *cpu);

void CPU_Dtor(CPU *cpu);

int Process(char *filename);

#endif // VM
