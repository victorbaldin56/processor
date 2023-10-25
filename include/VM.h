#ifndef VM
#define VM

#include <stdio.h>
#include "stack.h"
#include "codector.h"

#define DEBUG

typedef struct {
	Stack stack;
	double regs[NUM_REGS] = {};
} CPU;

CPU *CPU_Ctor(CPU *cpu);

void CPU_Dtor(CPU *cpu);

int Process(char *filename);

#endif // VM
