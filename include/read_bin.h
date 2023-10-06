#ifndef READBIN
#define READBIN

#include <stdio.h>
#include <inttypes.h>
#include "VM.h"
/// @brief Reads a binary file with bytecode to an array
Code *read_bin(FILE *input_file, Code *code_array);

#endif // READBIN
