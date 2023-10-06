#include "../include/read_bin.h"
#include "../include/VM.h"
#include <stdlib.h>
#include <assert.h>

#ifdef DEBUG
static void print_Code(const Code *code_array);
#endif

Code *read_bin(FILE *input_file, Code *code_array) {
    assert(input_file);

    code_array->size = 0; ///< size of code array

    while (fscanf(input_file, "%*s") != EOF) {
        code_array->size++;
    }

    rewind(input_file);

    code_array->code = (unsigned char *)calloc(code_array->size, 1);

    for (size_t i = 0; i < code_array->size; i++) {
        fscanf(input_file, "%hhx", code_array->code + i);
    }

    ON_DEBUG(printf("read_bin: parsed\n"));
    ON_DEBUG(print_Code(code_array));

    fclose(input_file);

    return code_array;
}

#ifdef DEBUG
static void print_Code(const Code *code_array) {
    assert(code_array);
    assert(code_array->code);

    printf("size = %zu\n", code_array->size);

    for (size_t i = 0; i < code_array->size; i++) {
        printf("%hhx ", code_array->code[i]);
    }

    putchar('\n');
}
#endif
