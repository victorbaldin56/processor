#include "VM.h"
#include <stdlib.h>
#include <assert.h>

Code *CodeCtor(Code *code_array, ssize_t code_size) {
    assert(code_array);
    assert(code_size > 0);

    code_array->size = code_size;
    code_array->code = (double *)calloc((size_t)code_size, sizeof(double));

    if (!code_array->code) return NULL;

    return code_array;
}

void CodeDtor(Code *code_array) {
    assert(code_array);

    free(code_array->code);
    code_array->code = NULL;
    code_array->size = 0;
}
