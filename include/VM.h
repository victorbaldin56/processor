#ifndef VM
#define VM

#include <stdio.h>
#include "stack.h"
#include "codector.h"

#define DEBUG

enum ExecRes {
	FILE_NOT_FOUND = -1,
	EXEC_OK = 0,
	NOT_VALID_FMT = 1,
};

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

ExecRes Process(char *filename);

#endif // VM
