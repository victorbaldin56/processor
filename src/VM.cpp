#include "stack.h"
#include "colors.h"
#include "VM.h"
#include <assert.h>
#include <math.h>
#include <signal.h>
#include "read_bin.h"
#include <stdlib.h>

#define SIGN(x) ((x > 0) - (x < 0))

#define Pop_(stk, val)                          \
    if (Pop(stk, val) == POP_EMPTY) {           \
        fprintf(stderr, "Pop: empty stack\n");  \
        abort();                                \
    }

#define Push_(stk, val)                         \
    if (Push(stk, val) == REALLOC_FAILED) {     \
        fprintf(stderr, "Push: push failed\n"); \
        abort();                                \
    }

#define DEF_CMD(name, opcode, has_arg, ...)     \
    case opcode:                                \
        __VA_ARGS__                             \


static const size_t MAXSTR = 1000; ///< max string lenght for byte-code
static const double EPS = 1e-7;    ///< floating point comparizon precision

static double get_arg(const Code *code_array, ssize_t *ip, CPU *cpu);

static int cmp_double(const double a, const double b, const double eps);

/// @brief executes a command by code
static int cmd_exec(const Code *code, ssize_t *ip, CPU *cpu);

static void vm_run(const Code *code_array);

void VM_Proc(char *filename) {
    assert(filename);

    Code code_array = {};
    read_bin(filename, &code_array);
    vm_run(&code_array);
    CodeDtor(&code_array);
}

static void vm_run(const Code *code_array) {
    assert(code_array);
    assert(code_array->code);

    CPU cpu = {};
    CPU_Ctor(&cpu);

    for (ssize_t ip = 0; ip < code_array->size; ip++) {
        if (cmd_exec(code_array, &ip, &cpu)) break;
    }

    CPU_Dtor(&cpu);
    return;
}

static int cmd_exec(const Code *code_array, ssize_t *ip, CPU *cpu) {
    assert(cpu);
    assert(ip);
    assert(code_array);
    assert(cpu->regs);
    STACK_ASS(&cpu->stack);
    assert((*ip) >= 0);

    // double arg1 = 0, arg2 = 0;
    char opcode = *(char *)(code_array->code + (*ip));

    ON_DEBUG(printf("opcode = %hhx\n", opcode & CMD));

    switch (opcode & CMD) {
        #include "commands.h"

        default:
        {
            fprintf(stderr, "Unknown command\n");
            abort();
        }
    }

}

#undef DEF_CMD

static double get_arg(const Code *code_array, ssize_t *ip, CPU *cpu) {
    assert(code_array);
    assert(ip);
    assert(cpu);

    double res = 0;

    char cmd = (char )code_array->code[*ip];

    if (cmd & IMM) {
        (*ip)++;
        res = code_array->code[*ip];
    }

    if (cmd & REG) {
        (*ip)++;
        res = cpu->regs[(size_t)code_array->code[*ip]];
    }

    return res;
}

static int cmp_double(const double a, const double b, const double eps) {
    assert(isfinite(a));
    assert(isfinite(b));
    assert(isfinite(eps));

    if (abs(b - a) < eps) {
        return 0;
    }

    return SIGN(b - a);
}
