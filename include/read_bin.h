#ifndef READBIN
#define READBIN

#include <stdio.h>
// #include <inttypes.h>
#include "VM.h"
#include "codector.h"
/// @brief Reads a binary file with bytecode to an array
Code *read_bin(char *filename, Code *code_array);

#endif // READBIN
