#ifndef VM
#define VM

#include <stdio.h>

#define DEBUG

#ifdef DEBUG
#define ON_DEBUG(x) x
#else
#define ON_DEBUG(x)
#endif

void VM_Proc(FILE *inpf);

#endif // VM
