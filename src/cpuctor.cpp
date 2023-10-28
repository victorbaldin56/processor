#include "stack.h"
#include "VM.h"

CPU *CPU_Ctor(CPU *cpu) {
    assert(cpu);

    StackCtor(&cpu->stack);
    StackCtor(&cpu->callstack);

    return cpu;
}

void CPU_Dtor(CPU *cpu) {
    assert(cpu);

    StackDtor(&cpu->stack);
    StackDtor(&cpu->callstack);
}
